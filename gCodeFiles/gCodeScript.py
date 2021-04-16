def printZero(xPad, yPad, xScale, yScale):
  x = xPad
  y = yPad
  print("\nThis is a zero: \n")
  markerUp();
  gotToStart(x, y);

  markerDown();
  y = verticalBarDown(x, y, yScale);
  y = verticalBarDown(x, y, yScale);
  x = horizontalBarRight(x, y, xScale);
  y = verticalBarUp(x, y, yScale);
  y = verticalBarUp(x, y, yScale);
  x = horizontalBarLeft(x, y, xScale);



def printOne(xPad, yPad, xScale, yScale):
  x = xPad
  y = yPad
  print("\nThis is a one:\n")
  markerUp();
  gotToStart(x, y);

  markerDown();
  y = verticalBarDown(x, y, yScale);
  y = verticalBarDown(x, y, yScale);



def gotToStart(xPad, yPad):
  gCode = "G1 X" + str(xPad) + " Y" + str(yPad)
  gCodeOut(gCode);

def horizontalBarRight(x, y, xScale):
  x = x + 50 * xScale
  gCode = "G1 X" + str(x) + " Y" + str(y)
  gCodeOut(gCode);
  return x;

def horizontalBarLeft(x, y, xScale):
  x = x - 50 * xScale
  gCode = "G1 X" + str(x) + " Y" + str(y)
  gCodeOut(gCode);
  return x;

def verticalBarUp(x, y, yScale):
  y = y - 100 * yScale
  gCode = "G1 X" + str(x) + " Y" + str(y)
  gCodeOut(gCode);
  return y;

def verticalBarDown(x, y, yScale):
  
  y = y + 100 * yScale
  gCode = "G1 X" + str(x) + " Y" + str(y)
  gCodeOut(gCode);
  return y;
def gCodeOut(gCode):
  print(gCode);

def markerUp():
  #the marker is not in contact with the whiteboard
  print("The marker is up")
  return;

def markerDown():
  #the marker is in contact with the whiteboard
  print("The marker is down")
  return;

printZero(25, 25, 1, 1)
printOne(25, 25, 1, 1);

