import serial
port = serial.Serial('COM4', 115200)
valuesBeforeAveraging = 6
numberOfSensors = 3
splitValues = []
nan = ['NAN','NA']

def getNewValue():
	newRead = port.readline()[:-2]
	print newRead
	# if nan[0] or nan[1] in newRead:
	# 	print "FIFO Buffer Overflow"
	# 	#print newRead
	# 	getNewValue()
	# else:
	return newRead

def getNewAverage(newValue):
	global splitValues
	average = 0.0
	outputLine = ""
	averageSetLength = len(splitValues)
	if averageSetLength == valuesBeforeAveraging:
		del splitValues[0]
	print averageSetLength
	splitValues.append([]) # Append new trial

	for yprNum, ypr in enumerate(newValue.split(":")): # For each sensor/YPR set
		splitValues[averageSetLength].append([]) # Append new array 

		for valueNum, value in enumerate(ypr.split(",")): # For each value in YPR set
			splitValues[averageSetLength][yprNum].append(float(value)) # Append value to array

			for entryNum in xrange(0, averageSetLength + 1): #For all trials
				average += splitValues[entryNum][yprNum][valueNum]	# Add all x,y,z values
			average = average / (averageSetLength + 1) # Divide by number of trials
			outputLine = outputLine + str(average)[:str(average).find('.') + 4] + ","
			average = 0.0
		outputLine = outputLine[:-1] + ":"
	outputLine = outputLine[:-1]

	return outputLine

while 1:
	try:
		file = open('Transfer.txt','w')
		file.write('')

		newRead = getNewValue()
		final = getNewAverage(newRead)
		print final
		file.write(final)
		file.close()
	except: IOError
