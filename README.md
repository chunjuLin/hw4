# hw4
(1) how to setup and run your program 

4_1: we will have three different input, d1, d2, and direction. We have to make the car move into the blank. We seperate
the movement into three part: move close, turn and move in. Move close is to move "d1", so that the car will be clsose
to the blank. Later on, we use the turn function, it depend on the direction we key in, it will make the the car parallel
with the blank. For move in we will move "d2" plus the length of the blank. We use encoder to count the length.
4_2: we want to make the car follow the line. So first of all we need to make the open MV only see the line I want. I figure 
out that we have to see too high, or it will see a lot of not necessary line. Also , we change the merge value, so that we will 
not see alot of line in one line. We use uart to send the message. we look at the x1, y1, x2, y2, of the line. If the y1 - y2
of the line has much different, if the different is negative we turn right, if the value is positive we turn left. So that 
the car will keep following the line.
4_3: we want to make the car approach the april tag. First of all, we have to send the angle of y and distance z,x to the uart.
we will use these parameter to judge if the car is approaching the apriltag. if the angle of the apriltag is larger than 355 or
less than 10 degree. we move foward. If larger, we first turn and move fowrd. If smaller, we turn and approach a little. So that
it approach the tag. However,if the ping say, it is less than 15 cm. We stop.  

 
(2) what are the results
4_1: the car will move properly by the parameter i key in.
4_2 the car will follow the line before the line end.
4_3: the car will follow the apriltag, and stop at roughing 15cm.
