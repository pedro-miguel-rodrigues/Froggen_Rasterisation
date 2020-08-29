var loader = new THREE.TextureLoader();
loader.crossOrigin = "";

var TableTexture1 = loader.load('https://i.imgur.com/zRET6e6.png');
var TableTexture2 = loader.load('https://i.imgur.com/M9i6Phj.png');
var TableBumpTex = loader.load('https://i.imgur.com/gfDGI2L.png');

var table_material = tableMaterial(TableTexture1, TableTexture2, TableBumpTex);

function TableDown(){
  var position = new THREE.Vector3( 0, -50, 0);

  // Table Geometry
  var geometry = new THREE.BoxBufferGeometry(200, 80, 0.5);
  THREE.BufferGeometryUtils.computeTangents( geometry );

  geometry.verticesNeedUpdate = true;
  geometry.elementsNeedUpdate = true;
  geometry.uvsNeedUpdate = true;
  geometry.computeFaceNormals();


  var mesh = new THREE.Mesh( geometry, table_material );
  GameObject.call(this, position, mesh, table_material, table_material, table_material);
  mesh.receiveShadow = true;
}


function tableMaterial(texture1, texture2, bumpMap) {
  var uniforms = {
    texture1: { type: "t", value: texture1 },
    //texture2: { type: "t", value: texture2 },
    bumpMap: { type: "t", value: bumpMap },
    material: { value: {
        diffuse: new THREE.Vector4(0.8, 0.9372, 0.4902, 1.0),
        specular: new THREE.Vector4(0.8, 0.9372, 0.4902, 1.0),
        shininess: 100,
        twoTextures: true,
        opacity: 0.5,
        transparent: true,
        blendSrcAlpha: 0.5
      }
    },
    fog: true
  }

  return new THREE.ShaderMaterial({
    uniforms: uniforms,
    vertexShader: vertexShader,
    fragmentShader: fragmentShader
  });
}