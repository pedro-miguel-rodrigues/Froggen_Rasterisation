var loader = new THREE.TextureLoader();
loader.crossOrigin = "";

var ButterTexture1 = loader.load('https://i.imgur.com/zRET6e6.png');
var ButterTexture2 = loader.load('https://i.imgur.com/M9i6Phj.png');
var ButterBumpTex = loader.load('https://i.imgur.com/gfDGI2L.png');

var butter_material = butterMaterial(ButterTexture1, ButterTexture2, ButterBumpTex);

function Butter(position){
  'use strict';

  var self = this;
  
  var geometry = new THREE.BoxBufferGeometry( 10, 15, 10 );
  THREE.BufferGeometryUtils.computeTangents( geometry );

  var material = butter_material;
  var mesh = new THREE.Mesh( geometry, material );
  SolidObject.call(this, position, mesh, new AxisAlignedBox(
    new THREE.Vector3(5, 7.5, 5),
    new THREE.Vector3(-5, -7.5, -5)
  ), butter_material, butter_material, butter_material);
}

function butterMaterial(texture1, texture2, bumpMap) {
  var uniforms = {
    texture1: { type: "t", value: texture1 },
    texture2: { type: "t", value: texture2 },
    bumpMap: { type: "t", value: bumpMap },
    material: { value: {
        diffuse: new THREE.Vector4(0.9529, 0.9372, 0.4902, 1.0),
        specular: new THREE.Vector4(0.9529, 0.9372, 0.4902, 1.0),
        shininess: 100,
        twoTextures: true
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

function updateButterFog(scene){
  if(scene.fog != null){
    butter_material.uniforms.fogColor = { type: "c", value: scene.fog.color };
    butter_material.uniforms.fogDensity = { type: "f", value: scene.fog.density };
    //butter_material.uniforms.enableFog = { type: "b", value: true };
  } else {
    //butter_material.uniforms.enableFog = { type: "b", value: false };
  }
}