## BBInvaders
_Detailed description is on its way_


### Remarks
Originally I wanted _asteroids_ and _invaders_ to be made with instanced static mesh component to improve perfomance, but for some reason generated instances were missing collision. 
Like, at all. And I honestly no idea why, since the mesh itself had collision and the component setup should be the same as with usual static mesh component (well, except _ISMComponent->bMultiBodyOverlap = true;_).
![demo picture](https://i.imgur.com/3YJ0LnB.png)
I could _assign_ collision components to each instance, but decided to stick with regular SM components. If you have any ideas what I missed, suggestions would be highly appreceated.

### Compatibility
* Unreal Engine 4.27
* C++20 standart is required to build game module. Corresponding UBT files for UE4 can be found [here](https://github.com/dimkKy/UBT4-CPP20/releases/tag/1.0)
