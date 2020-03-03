# Impossible Rescue
 A 2.5D single screen sidescrolling platformer created as part of the first GamerCamp Masters 2019 collaboratively developed games in an industry development environment.
 
 This game's platforming and hacking mechanics are reminiscent of Manic Miner(Bug-Byte, 1983) and Impossible Mission(Epyx, 1984) with a more up-to-date polishing, and the art style is designed to reflect a 2D re-imagination of Shadow Complex(Chair Entertainment, 2009).
 
 As Danielle, you'll use your trusty robot companion SHADOW to traverse a Martian security complex, hacking its systems to unlock new areas and save the president of Earth's daughter.
 
 Launch the game by downloading or cloning this repo and navigating to [the Game folder](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Game), and running [Impossible Rescue.exe](https://github.com/flyscript/Impossible-Rescue/blob/master/ImpossibleRescue-Game/Impossible%20Rescue.exe)
 
 ![Image of Yaktocat](https://static.wixstatic.com/media/3978e6_c6bd3535adea42bb963bdf656ea1835cf003.jpg/v1/fill/w_756,h_425,fp_0.50_0.50,q_90/3978e6_c6bd3535adea42bb963bdf656ea1835cf003.webp)
 

### Key Features:
* [Terminals that can be "Hacked"](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Hackables) via quick-time-events or time-based effort
* A following [robot companion](https://github.com/flyscript/Impossible-Rescue/blob/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Player/CObjShadow.cpp) that you can use as a rechargable weapon
* 3 different [enemies](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Enemies) and behaviours
* Ability to [save and load](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Source/Classes/GamerCamp/GameController/DataPersistence) progress
* [Moving platforms](https://github.com/flyscript/Impossible-Rescue/blob/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Platforms/CObjMovingPlatform.cpp)
* [Crumbling platforms](https://github.com/flyscript/Impossible-Rescue/blob/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Platforms/CObjCrumblingPlatform.cpp)
* 2D [platforming and player movement](https://github.com/flyscript/Impossible-Rescue/blob/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Player/GCObjPlayer.cpp) mechanics
* [Time-based](https://github.com/flyscript/Impossible-Rescue/blob/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/Levels/Act_1/CLevel4.cpp) levels
* An informative and unobtrusive [UI](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/UI)
* A [tutorial/narrative system](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Source/Classes/GamerCamp/GameSpecific/ShadowTutorial) that conveys story and game mechanic elements
* An [event management system](https://github.com/flyscript/Impossible-Rescue/tree/master/ImpossibleRescue-Source/Classes/GamerCamp/EventManager) that gives the player the ability to interact with the world, and allows world elements to affect eachother
