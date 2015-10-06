import serial
port = serial.Serial('COM4', 115200)
valuesBeforeAveraging = 6
numberOfSensors = 3
splitValues = []
averageSetLength = 0
nan = ['nan','na']

def getNewValue():
	newRead = port.readline()[:-2].lower()
	if nan[0] in newRead or nan[1] in newRead:
		print "FIFO Buffer Overflow"
		getNewValue()
	else:
		return newRead

def getNewAverage(newValue):
	global splitValues
	global averageSetLength
	average = 0.0000
	outputLine = ""
	
	splitValues.append([]) # Append new trial
	
	if averageSetLength > valuesBeforeAveraging:
		del splitValues[0]
		
	averageSetLength = len(splitValues)
	
	for yprNum, ypr in enumerate(newValue.split(":")): # For each sensor/YPR set
		splitValues[averageSetLength - 1].append([]) # Append new array, compensate for array start 0

		for valueNum, value in enumerate(ypr.split(",")): # For each value in YPR set
			splitValues[averageSetLength - 1][yprNum].append(float(value)) # Append value to array
			
			for entryNum in xrange(0, averageSetLength): #For all trials
				average += splitValues[entryNum][yprNum][valueNum] # Add all x,y,z values
			average = average / (averageSetLength) # Divide by number of trials
			outputLine = outputLine + str(average)[:str(average).find('.') + 4] + ","
			average = 0.0000
		outputLine = outputLine[:-1] + ":"
	outputLine = outputLine[:-1]

	return outputLine

while 1:
	try:
		newRead = getNewValue()
		final = getNewAverage(newRead)
		print final
		file = open('Transfer.txt','w')
		file.write(final)
		file.close()
	except: IOError
