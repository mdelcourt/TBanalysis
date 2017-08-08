import os
import sys

#files = ["AnalysisTree_0928.root","AnalysisTree_0933.root","AnalysisTree_0934.root","AnalysisTree_0935.root","AnalysisTree_0936.root","AnalysisTree_0937.root","AnalysisTree_0938.root","AnalysisTree_0940.root","AnalysisTree_0941.root"]

files =["AnalysisTree_0857.root","AnalysisTree_0859.root","AnalysisTree_0862.root","AnalysisTree_0864.root","AnalysisTree_0865.root","AnalysisTree_0866.root","AnalysisTree_0867.root","AnalysisTree_0868.root","AnalysisTree_0870.root","AnalysisTree_0871.root","AnalysisTree_0872.root","AnalysisTree_0873.root","AnalysisTree_0874.root","AnalysisTree_0880.root","AnalysisTree_0881.root","AnalysisTree_0885.root","AnalysisTree_0886.root","AnalysisTree_0889.root","AnalysisTree_0892.root","AnalysisTree_0893.root","AnalysisTree_0894.root","AnalysisTree_0895.root","AnalysisTree_0900.root","AnalysisTree_0905.root","AnalysisTree_0906.root","AnalysisTree_0926.root","AnalysisTree_0927.root","AnalysisTree_0928.root","AnalysisTree_0933.root","AnalysisTree_0934.root","AnalysisTree_0935.root","AnalysisTree_0936.root","AnalysisTree_0937.root","AnalysisTree_0938.root","AnalysisTree_0940.root","AnalysisTree_0941.root","AnalysisTree_0942.root","AnalysisTree_0943.root","AnalysisTree_0944.root","AnalysisTree_0969.root","AnalysisTree_0971.root","AnalysisTree_0972.root","AnalysisTree_0973.root","AnalysisTree_0974.root","AnalysisTree_0975.root","AnalysisTree_0978.root","AnalysisTree_0979.root","AnalysisTree_0980.root","AnalysisTree_0981.root","AnalysisTree_0982.root","AnalysisTree_0983.root","AnalysisTree_0984.root","AnalysisTree_0985.root","AnalysisTree_0986.root","AnalysisTree_0987.root","AnalysisTree_0988.root","AnalysisTree_0989.root","AnalysisTree_0990.root"]
inputPath = "/home/ucl/cp3/delcourt/storage/tbFullTree/1611/"
inputFile = ""
if sys.argv[-1].isdigit():
  inputFile = inputPath+files[int(sys.argv[-1])]
elif os.environ["fileId"].isdigit():
    inputFile = inputPath+files[int(os.environ["fileId"])]
else:
  print "Expected file id (length = %s)"%len(files)
  sys.exit()

fileId    = inputFile.split(".")[-2][-3:]
print "Run number = %s"%fileId
print "pwd :"
os.system("pwd")
print "ls :"
os.system("ls -lrth")

steps = ["firstSetOfConstants","fineTune","effMeas"]
consumed = "None"
for s in steps:
  produced = "Run%s_%s.root"%(fileId,s)
  ex = "bin/%s.out %s %s %s"%(s,inputFile, consumed,produced)
  print ex
  consumed = produced
  os.system(ex)





