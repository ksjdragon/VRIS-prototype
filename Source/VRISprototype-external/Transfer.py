from decimal import Decimal
import serial
port = serial.Serial('COM4', 115200) #Serial Port, Baud Rate
global valuesBeforeAveraging #Iterations before average
global numberOfSensors
global rawValues
global splitValues
global averagedValues
valuesBeforeAveraging = 6
numberOfSensors = 3
rawValues = []
splitValues = []
averagedValues = []

for a in xrange(0, valuesBeforeAveraging):
	rawValues[a] = port.readline() #Assign first few values
	splitValues.append([]) #Dynamic multidimensional array
	for b in xrange(0, numberOfSensors):
		splitValues[a].append([]) #Dynamic multidimensional array
		averagedValues.append([])

def getAverage():
	return toString(average(split(getNewValue())))

def getNewValue():
	print ser.readline()
	rawValues.append(ser.readline())
	rawValues.pop(0)
	return rawValues
		
def split(valuesToSplit):
	for entryNum, entry in enumerate(valuesToSplit):
		for yprNum, ypr in enumerate(newEntry.split(":")):
			for valueNum, value in enumerate(values.split(",")):
				splitValues[entryNum][yprNum][valueNum] = Decimal(value)
	return splitValues
 
def average(valuesToAverage):
	averagedValue = Decimal(0.0)
	for yprSetNum, yprSet in enumerate(valuesToAverage[0]):
		for valueNum, value in enumerate(yprSet):
			for entryNum in xrange(0,6):
				averagedValue += averagedValue + Decimal(valuesToAverage[yprSetNum][valueNum][entryNum]) 
			averagedValue = averagedValue / 6
			averagedValue = averagedValues[valueNum][entryNum]
			averagedValue = Decimal(0)
	return averagedValues

def toString(averagedValues):
	print "toString"
	outputLine = ""
	for ypr in averagedValues:
		for value in ypr:
			outputLine = outputLine + str(value) + ","
		outputLine = outputLine + ":"
	outputLine = outputLine[:-1]
	return outputLine

while 1:
        try:
                file = open('Transfer.txt', 'w')
		file.write("")
		final = getAverage()
		print final
		file.write(final)
		file.close()
	except: IOError
		
# Data structure for splitValues
# [
#   [
#     [value,value,value],
#     [value,value,value],
#     [value,value,value],
#   ],
#   [
#     [value,value,value],
#     [value,value,value],
#     [value,value,value],
#   ],
#   [
#     [value,value,value],
#     [value,value,value],
#     [value,value,value],
#   ],
#   [
#     [value,value,value],
#     [value,value,value],
#     [value,value,value],
#   ],
#   [
#     [value,value,value],
#     [value,value,value],
#     [value,value,value],
#   ],
#   [
#     [value,value,value],
#     [value,value,value],
#     [value,value,value],
#   ],
# ]
