import time

##get data count from file
dataCountFile = open("dataCountFile.txt","r")
dataCount = int(dataCountFile.read())
print("dataCount is:", dataCount)
dataCountFile.close

while True :
    ##get data count from file
    dataCountFile = open("dataCountFile.txt","w")
    dataCount +=1 
    dataCountFile.write(str(dataCount))
    print("dataCount is:", dataCount)
    dataCountFile.close
    time.sleep(2)