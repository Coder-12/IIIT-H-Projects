# QUERY = """Products_3 JOIN Categories JOIN Inventories_1 UNION Products_4 JOIN Categories JOIN Inventories_1 UNION Products_2 JOIN Categories JOIN Inventories_1 UNION Products_1 JOIN Categories JOIN Inventories_1 UNION Products_3 JOIN Categories JOIN Inventories_3 UNION Products_4 JOIN Categories JOIN Inventories_3 UNION Products_2 JOIN Categories JOIN Inventories_3 UNION Products_1 JOIN Categories JOIN Inventories_3 UNION Products_3 JOIN Categories JOIN Inventories_4 UNION Products_4 JOIN Categories JOIN Inventories_4 UNION Products_2 JOIN Categories JOIN Inventories_4 UNION Products_1 JOIN Categories JOIN Inventories_4 UNION Products_3 JOIN Categories JOIN Inventories_2 UNION Products_4 JOIN Categories JOIN Inventories_2 UNION Products_2 JOIN Categories JOIN Inventories_2 UNION Products_1 JOIN Categories JOIN Inventories_2"""
 
# if __name__ == "__main__":
#     ATTRIBUTES_DATATYPE = getAttributesDatatype()
#     predicate = "Products.categoryID!=1 and Products.listPrice>5000 and Products.productDescription != 'a' "
#     VAR_CONDITIONALS = getVarConditionals(predicate)
#     predicate = preParsePredicate("""(Products.categoryID=1 or Products.listPrice>5000) and Products.categoryID=1 or Products.productDescription!="b" """)

#     print(evaluate(predicate, VAR_CONDITIONALS, ATTRIBUTES_DATATYPE))