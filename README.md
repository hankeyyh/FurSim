FurSim
=====================
A practice project about Hair simulation based on [Fast Simulation of Inextensible Hair and Fur](http://matthias-mueller-fischer.ch/publications/FTLHairFur.pdf). 
The project includes a simple wind field and a implementation of collision handling. UI is from [nanogui](https://github.com/wjakob/nanogui).
### Limit
1. Hair shader is simple, maybe `The Marschner Reflectance Model` can help.
2. Collision handling algorithm is also simple, now it uses a sphere-sphere collision model, if a hair srand contacts the head sphere, just give a force along the normal dirction of contact point. 
3. Now just few hair srands are simulated (about 540), more hair can be handled by introducing cuda.
****
![](https://github.com/hanke100321/FurSim/blob/master/resources/ScreenShots/hair1.png)
![](https://github.com/hanke100321/FurSim/blob/master/resources/ScreenShots/hair3.png)
