README
=====================
A practice project about Hair simulation based on [Fast Simulation of Inextensible Hair and Fur](http://matthias-mueller-fischer.ch/publications/FTLHairFur.pdf). 
The project includes a simple wind field and a implementation of collision handling. UI is from [nanogui](https://github.com/wjakob/nanogui).
### Limit
1. Hair shader is simple, maybe `The Marschner Reflectance Model` can help
2. Collision handling algorithm is also simple, now it use a sphere-sphere collision model, if a srand of hair contacts the head sphere, just give
a force along the normal of contact point. 
****
![]()
