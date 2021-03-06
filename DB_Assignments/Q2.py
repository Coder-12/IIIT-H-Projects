import pyspark
from pyspark.sql import *
from pyspark.sql.functions import col
import argparse

parser = argparse.ArgumentParser(description='solver')
parser.add_argument('outpath',type=str,metavar='O',help='path to output')
parser.add_argument('--ncpu',type=int,default=2,help='number of cpus')

args = parser.parse_args()

sc = SparkSession.builder.master('local').appName('Word Count').config('spark.some.config.option','some-value').getOrCreate()

airports = sc.read.csv('Dataset/airports.csv',header=True,inferSchema=True)

df = airports.groupby("COUNTRY").count()
df = df.sort(col("count").desc())

df.toPandas().to_csv(args.outpath,index=False)
