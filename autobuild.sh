display_ppm () { display ./out.tga & display ./z.tga; }
OBJ=${1:-diablo3_pose}
./rebuild_cmake ; cd build ; ./diabolo-anaglyphe-rasterizer $OBJ; display_ppm & echo "done"
