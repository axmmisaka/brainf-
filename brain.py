# Note this is not 100% brainfuck, since the size of the list and elements of the list is not byte. Consider it an improved one.
import copy
import colorama

# Make colors available on M$ Windows
colorama.init()

LEFT = -1
RIGHT = 1
ADD = 1
MINUS = -1

class program(object):
    def __init__(self,inputCode, initialData = [0], initialDataLocation = 0):
        self.code = inputCode
        self.data = initialData[:]
        self.dataLocation = initialDataLocation
        self.codeLocation = 0

    # Handles < and > 
    # Direction should be LEFT or RIGHT
    def modifyDataPointer(self, direction):
        if direction == LEFT and self.dataLocation == 0:
            # Add an element at the beginning
            self.data.insert(0,0) 
        elif (direction == RIGHT) and (self.dataLocation == (len(self.data) - 1)):
                self.data.append(0) # Add an element at the end
                self.dataLocation += 1
        else: # normal situation
        #    if direction == LEFT:
        #        dataLocation -= 1
        #    else:
        #        dataLocation += 1
            self.dataLocation += direction

    # Handles + and -
    # Value should be ADD or MINUS 
    def modifyDataValue(self, value):
        self.data[self.dataLocation] += value
    
    # Handles ,
    def inputData(self):
        print("Please input data...: ")
        inputValue = input()
        self.data[self.dataLocation] = inputValue

    # Handles .
    def outputData(self):
        print(chr(self.data[self.dataLocation]),end = '')

    # Handles []
    # Return value:
    # modified data
    def subCode(self, subCodeString):
        newProgram = program(subCodeString, self.data, self.dataLocation)
        modifiedData = newProgram.interprete()
        return modifiedData
    
    # data[startLocation] should be '['
    def findSubCode(self, startLocation):
        stack = 0 # when stack == 0, then the corresponding subcode is found
        modifier = {'[':1, ']':-1}
        for i in range(startLocation, len(self.code)):
            if self.code[i] in modifier: # if the code is [ or ]
                stack += modifier[self.code[i]] # add or minus "stack"
            
            if stack == 0:
                return self.code[startLocation + 1 : i + 1]
            
            assert stack >= 0, "Syntax Error: Bad Nesting"
        assert True, "Syntax Error: Bad Nesting"

    # Return: pointer needed to be modified and data(if any)
    def interpreteCurrentStep(self):
        if self.code[self.codeLocation] == '+':
            self.modifyDataValue(ADD)
            return (1,)
        elif self.code[self.codeLocation] == '-':
            self.modifyDataValue(MINUS)
            return (1,)
        elif self.code[self.codeLocation] == '<':
            self.modifyDataPointer(LEFT)
            return (1,)
        elif self.code[self.codeLocation] == '>':
            self.modifyDataPointer(RIGHT)
            return (1,)
        elif self.code[self.codeLocation] == ',':
            self.inputData()
            return (1,)
        elif self.code[self.codeLocation] == '.':
            self.outputData()
            return (1,)
        elif self.code[self.codeLocation] == '[':
            newCode = self.findSubCode(self.codeLocation)
            # move the code pointer forward first
            self.codeLocation += len(newCode) + 1
            # if nonzero, new subprogram
            if self.data[self.dataLocation] != 0:
                self.data = self.subCode(newCode)
            return (0,)
        # TODO: Logic is wrong, this could only serve one loop
        elif self.code[self.codeLocation] == ']':
            if self.data[self.dataLocation] != 0:
                #Right after corresponding [
                self.codeLocation = 0
                return(0,)
            else:
                return (1,self.data[:])
        else:
            assert True, "Unexpected character{0}".format(self.code[self.codeLocation])

    def interprete(self):
        returnVal = 0
        while self.codeLocation < len(self.code):
            #print(self)
            #visulizeCode(self)
            #visulizeData(self)
            returnVal = self.interpreteCurrentStep()
            self.codeLocation += returnVal[0]
            #input()
        
        return 0 if len(returnVal) == 1 else returnVal[1]

def visulizeCode(bfProgramObject):
    print(bfProgramObject.code[0 : bfProgramObject.codeLocation],end='')
    print(colorama.Fore.RED + bfProgramObject.code[bfProgramObject.codeLocation] + colorama.Style.RESET_ALL,end='')
    print(bfProgramObject.code[bfProgramObject.codeLocation + 1: len(bfProgramObject.code)],end = '')
    print('')

def visulizeData(bfProgramObject):
    for i in range(0,len(bfProgramObject.data)):
        isPointing = (i == bfProgramObject.dataLocation)
        if isPointing:
            print(colorama.Fore.RED,end = '')
        print(bfProgramObject.data[i],end = ' ')
        if isPointing:
            print(colorama.Style.RESET_ALL, end = '')
    print('')
        
