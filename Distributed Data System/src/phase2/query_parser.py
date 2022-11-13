from moz_sql_parser import parse
from copy import deepcopy
import json
import config
import requests
import solve_predicates

import sys 
sys.setrecursionlimit(10**6)

def executeQuery(query):
    URL = config.SYS_CAT_QUERY_URL
    req_obj = {'query': query}
    query_response = requests.post(URL, json = req_obj)
    query_response_object = query_response.json()
    # print(query_response_object)
    return query_response_object['response']

def fetchFragments(relation):
    QUERY = """ {} "{}" """.format(config.FETCH_RELATION_FRAGS, relation)
    response = executeQuery(QUERY)
    fragments = set()
    for row in response:
        for frag_id in row:
            fragments.add(str(frag_id))
    return list(fragments)

def getAttributesDatatype():
    ATTRIBUTES_DATATYPE = {}
    query_response = executeQuery(config.FETCH_ALL_ATTRS_QUERY)
    for row in query_response:
        ATTRIBUTES_DATATYPE[row[3]] = row[-1]
        ATTRIBUTES_DATATYPE['{}.{}'.format(row[1],row[3])] = row[-1]
    return ATTRIBUTES_DATATYPE


def fetchFragmentAttributes(fragment_id):
    QUERY = """ {} {} """.format(config.FETCH_FRAG_ATTR_QUERY, fragment_id)
    response = executeQuery(QUERY)
    attributes = set()
    for row in response:
        for attr in row:
            attributes.add(attr)
    return attributes

def getFragmentType(fragment_id):
    QUERY = """ {} {} """.format(config.FETCH_FRAG_TYPE_QUERY, fragment_id)
    response = executeQuery(QUERY)
    frag_type = response[0][0]
    return frag_type

class Query(object):

    def __init__(self, query):
        super().__init__()
        self.FRAGMENT_PREDICATE_CACHE = {}
        self.KEYWORDS = config.QUERY_KEYWORDS

        self.query = self.parseQuery(query)

        self.QUERY_ATTRIBUTES = set()
        self.getAttributes()

        self.joins = None
        delim = ''
        self.generateJoins()

        print(self.joins)

        if 'where' in self.query.keys():
            query_where_clause = self.getQueryWhereClause(query)
            self.ATTRIBUTES_DATATYPE = getAttributesDatatype()
            self.joins = self.horizontalPruning(deepcopy(self.joins), query_where_clause)
            print(self.joins)
        # predicate = "Products.categoryID!=1 and Products.listPrice>5000 and Products.productDescription != 'a' "
        # VAR_CONDITIONALS = solve_predicates.getVarConditionals(predicate)
        # predicate = self.preParsePredicate("""(Products.categoryID=1 or Products.listPrice>5000) and Products.categoryID=1 or Products.productDescription!="b" """)
        # print(solve_predicates.evaluate(predicate, VAR_CONDITIONALS, ATTRIBUTES_DATATYPE))

    def preParsePredicate(self, predicate):
        op_replacements = {}
        op_replacements['='] = ' = '
        op_replacements['! ='] = ' != '
        op_replacements['>'] = ' > '
        op_replacements['>  ='] = ' >= '
        op_replacements['<'] = ' < '
        op_replacements['<  ='] = ' <= '
        op_replacements['('] = ' ( '
        op_replacements[')'] = ' ) '
        for op in ('=', '! =', '>', '>  =', '<', '<  =', '(', ')'):
            predicate = predicate.replace(op, op_replacements[op])
        return predicate

    def getQueryWhereClause(self, query):
        query_where_clause = query.split('where')[1].split('group')[0]
        return self.preParsePredicate(query_where_clause)

    def generateJoins(self):
        if type(self.query["from"]) is not list:
            self.query["from"] = [self.query["from"]]

        delim = ''
        
        for table in self.query["from"]:
            # print(table, end=" ----->")
            fragments = fetchFragments(table)

            frag_type = getFragmentType(fragments[0])
            if  frag_type == 'VF':
                delim = 'JOIN'
                fragments = self.verticalFragmentPruning(fragments)
            elif frag_type != 'NF':
                delim = 'UNION'
            
            if self.joins is None:
                self.joins = [delim, fragments]
            else:
                joins = []
                for frag in fragments:
                    joins.append( self.appendJoinToPrevJoin(deepcopy(self.joins), frag) )

                self.joins = [delim, joins]
        return

    def appendJoinParsingCheck(self, join):
        if type(join[0]) != list and join[0] not in ("UNION", "NATURAL_JOIN", "JOIN", ''):
            return True
        return False

    def appendJoinToPrevJoin(self, join, frag):
        if type(join) == str:
            return join
        elif type(join) == list:
            if self.appendJoinParsingCheck(join):
                intermed_joins = []
                for int_join in join:
                    intermed_joins.append(int_join+' JOIN '+frag)
                return intermed_joins
            else:
                join = [ self.appendJoinToPrevJoin(ele, frag) for ele in join ]
                return join

    def parseQuery(self, inputQuery):
        tokenizedQuery = json.loads( json.dumps( parse(inputQuery) ) )
        print("TOKENIZED_QUERY -----> ",tokenizedQuery)
        return tokenizedQuery

    def isAttribute(self, obj):
        if type(obj) is str and obj not in self.KEYWORDS:
            return True
        return False

    def getAttributes(self):
        obj = self.query
        for key, value in obj.items():
            if key.lower() != "from":
                self.getAttributesUtil(value)

    def getAttributesUtil(self, obj):
        if type(obj) is dict:
            for key, value in obj.items():
                if type(value) is dict:
                    self.getAttributesUtil(value) 
                elif type(value) is list:
                    for obj in value:
                        self.getAttributesUtil(obj)
                else:
                    self.getAttributesUtil(key)
                    self.getAttributesUtil(value)
        else:
            if self.isAttribute(obj):
                if obj.find('.')!=-1:
                    # relation.attr
                    self.QUERY_ATTRIBUTES.add(obj.split('.')[1])
                else:
                    self.QUERY_ATTRIBUTES.add(obj)

    def getFragmentPredicate(self, fragment_id):
        if fragment_id in self.FRAGMENT_PREDICATE_CACHE.keys():
            return self.FRAGMENT_PREDICATE_CACHE[fragment_id]
        predicate = executeQuery(config.COND_QUERY.format(fragment_id))
        if len(predicate) == 0:
            return None
        predicate = solve_predicates.preParsePredicate(predicate[0][0])
        self.FRAGMENT_PREDICATE_CACHE[fragment_id] = predicate
        return predicate


    def verticalFragmentPruning(self, fragments):

        # All Attributes Required, no pruning possible
        if '*' in self.QUERY_ATTRIBUTES:
            return fragments

        pruned_fragments = set()

        for frag in fragments:
            frag_attrs = fetchFragmentAttributes(frag)
            if self.QUERY_ATTRIBUTES.isdisjoint(frag_attrs) is False:
                pruned_fragments.add(frag)

        return pruned_fragments
        

    def getConditional(self, frag_id, attr):
        if attr.find('.')!=-1:
            attr = attr.split('.')[1]
        conditionals = executeQuery(config.COND_QUERY.format(frag_id, attr))
        if len(conditionals) == 0:
            return None
        return conditionals

    def evaluatePredicate(self, predicate, frag_id):
        if predicate.find('and')!=-1:
            return self.evaluatePredicate(predicate.split('and')[0], frag_id) and self.evaluatePredicate(predicate.split('and')[1], frag_id)
        if predicate.find('or')!=-1:
            return self.evaluatePredicate(predicate.split('or')[0], frag_id) or self.evaluatePredicate(predicate.split('or')[1], frag_id)
        else:
            lhs, operator, rhs = predicate.split(' ')
            lfrag_cond = getConditional(frag_id, lhs)
            rfrag_cond = getConditional(frag_id, rhs)
            if lhs in self.attrs:
                if lfrag_cond is None:
                    return True
            if rhs in self.attrs:
                if rfrag_cond is None:
                    return True
            return self.evaluate(lhs, operator, rhs)

    def getFragRelName(self, frag_id):
        for row in self.fragment_attrs_info:
            if row[0] == frag_id:
                return row[1]
        return None

    def getJoinAttr(self, fragments):
        f1_attrs = fetchFragmentAttributes(fragments[0])
        f2_attrs = fetchFragmentAttributes(fragments[1])
        join_attr = f1_attrs.intersection(f2_attrs)

        if join_attr.__len__()==0:
            return None

        return list(join_attr)[0]

    def horizontalPruning(self, join, query_where_clause):
        if type(join) == str:
            return join
        elif type(join) == list:
            if self.appendJoinParsingCheck(join):
                intermediate_joins = []
                for int_join in join:
                    frags = int_join.split('JOIN')
                    conditionals = '1=1'
                    for frag in frags:
                        if frag!='':
                            frag_pred = self.getFragmentPredicate(frag)
                            if frag_pred is not None:
                                conditionals = '{} and {}'.format(conditionals, frag_pred)
                    
                    if conditionals == '1=1':
                        intermediate_joins.append(int_join)
                        continue

                    VAR_CONDITIONALS = solve_predicates.getVarConditionals(conditionals)

                    if solve_predicates.evaluate(query_where_clause, VAR_CONDITIONALS, self.ATTRIBUTES_DATATYPE) == True:
                        intermediate_joins.append(int_join)
                return intermediate_joins
            else:
                join = [ self.horizontalPruning(ele, query_where_clause) for ele in join ]
                return join

    def fragmentJoinElimination(self):
        """
        PIPELINE:
        1. VerticalFragment(Attributes Absent) Pruning
        2. HorizontalFragment Pruning (Predicate Mismatch) 
        """
        # self.verticalPruning()
        # self.horizontalPruning()
        pass

 
query = input()

query = Query(query)