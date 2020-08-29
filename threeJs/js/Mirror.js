var cubeCamera;
var mirrorMesh;

function Mirror(scene, geometry, transform){  
    // Table Geometry
    var resolution = 1024;

    var loader = new THREE.TextureLoader();
    loader.crossOrigin = "";
  
    var texture = loader.load('https://i.imgur.com/h5FsFbl.png');

    var mirror = new THREE.Reflector( geometry, {
        clipBias: 0.003,
        color: 0x777777,
        recursion: 30,
        textureOne: texture,
        textureWidth: resolution,
        textureHeight: resolution,
        blendSrcAlpha: 0.5
    } );

    transform(mirror);


    scene.add( mirror );
}
