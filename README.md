# diabolo-anaglyphe-rasterizer : 2020 FST Renderer project
_Created by Alison Théo (Thelias), Alexis Cesaro (AlexisDevMaster)_

## How to build

The repository comes with two handy script files ; **rebuild_cmake** creates the build directory, executes CMake and drops the results inside. But if you want to go faster, executing **autobuild.sh** builds the project, executes it (by default on diablo3_pose) and then displays both the anaglyphed render and the red part's zbuffer.

You can also add an argument to **autobuild.sh** ; the program will look for \<arg>.obj and\<arg>\_diffuse.tga to render whatever model you may have.

## What to expect

Well, it's not to bad. It's not like the best renderer there may ever be but it's okay-ish. We never managed to have a functionning projection matrix but the code bits are still here just in case we (or anyone else) wants to try their luck (and to show how hard we tried).

We've got textures at least.

## Evolution over time

### First wireframe display (modulo errors look funny)
![First Wireframe](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/first_wireframe.png)

### Final wireframe display
![Final Wireframe](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/wireframe_final.png)

### First rasterization (modulo errors again)
![First Rasterization](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/first_rasterization.png)

### Rasterization fixed
![Fixed Rasterization](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/rasterization_unified.png)

### Rasterization with lightning
![Light Rasterization](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/rasterization_light.png)

### Colored rasterization
![Color Rasterization](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/rasterization_colors.png)

### Bounding Box rasterization (first tries with barycentric coordinates)
![BB Rasterization](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/rasterization_bb.png)

### Zbuffering
![Zbuffer](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/zbuffer.png)

### First tries with texturing
![First Texture](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/first_texture.png)

### Texturing fixed
![Texture final](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/texture_final.png)

### Anaglyph display
![Anaglyph](https://github.com/TheliasRosewind/diabolo-anaglyphe-rasterizer/blob/master/images/anaglyph.png)

## Final thoughts

This was a great project to do, even though we wish that we could've brought it further than where it is now. We'd like to thank [Dmitry Sokolov](https://github.com/ssloy) for his teachings as well as allowing us to have this great diablo model.
