[![discord](https://img.shields.io/discord/495955797872869376.svg?logo=discord "Discord")](https://discord.gg/YJyrvgQ)

# voxelquest

This is a restructuring of the VoxelQuest (https://www.voxelquest.com/) source code.

If you do decide to play with it be aware that it is setup to use hunter (https://github.com/cpp-pm/hunter). This means your first compile will download the current dependices and compile them. This may take some time as some of the dependencies are large (looking at you boost). This is a one time compile so after that the builds will be normal.

(Optional SFML)
For the moment, by default audion is disabled. If you want audio you will need to download the latest version of SFML and compile it. Call cmake with `-DVOXELQUEST_USE_SFML=ON -DSFML_ROOT_DIRECTORY={Root Directory of SFML}`. This build is expecting that you setup cmake to build SFML in the `build` directory using something like this: `cmake -H. -Bbuild` for the cmake option build directory.

Current screenshots
![screenshot](https://github.com/caseymcc/voxelquest/raw/master/screenshot.png)
![screenshot](https://github.com/caseymcc/voxelquest/raw/master/screenshot1.png)



