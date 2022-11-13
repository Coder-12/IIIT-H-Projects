# Database Connection AUTH
HOSTNAME = "localhost"
USERNAME = "user"
PASSWORD = "iiit123"
AUTH_PLUGIN = "mysql_native_password"
DB = 'TeamRandom'

OPERATORS = ["le", "lt", "ge", "gt", "eq"]
QUERY_KEYWORDS = set(["value", "select", "from", "where", "le", "lt", "ge", "gt", "eq"])

# IP Addresses
SYS_CAT_QUERY_URL = 'http://10.3.5.215:8081/query'

# Query
FETCH_FRAG_ATTR_QUERY = "Select DISTINCT attribute from FragmentsAttributesList, RelationAttributes where FragmentsAttributesList.attribute_id = RelationAttributes.attribute_id and fragment_id="
FETCH_RELATION_FRAGS = "Select fragment_id from Fragments where relation_name="

FETCH_ALL_ATTRS_QUERY = "select Fragments.fragment_id, relation_name, RelationAttributes.attribute_id, attribute, attribute_type from Fragments, FragmentsAttributesList, RelationAttributes where Fragments.fragment_id = FragmentsAttributesList.fragment_id and FragmentsAttributesList.attribute_id = RelationAttributes.attribute_id;"

COND_QUERY = """select predicate from Conditionals where fragment_id = {};"""

FETCH_FRAG_TYPE_QUERY = """ select fragment_type from Fragments where fragment_id = """