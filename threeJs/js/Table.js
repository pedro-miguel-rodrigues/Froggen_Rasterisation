function Table(render){
  var position = new THREE.Vector3( 0, 50, 0);

  // Table Geometry
  var geometry = new THREE.PlaneGeometry(220, 80);

  geometry.verticesNeedUpdate = true;
  geometry.elementsNeedUpdate = true;
  geometry.uvsNeedUpdate = true;
  geometry.computeFaceNormals();

  var loader = new THREE.TextureLoader();
  loader.crossOrigin = "";

  var texture1 = new THREE.TextureLoader().load( 'https://i.imgur.com/h5FsFbl.png' );

  var texture2 = new THREE.TextureLoader().load( 'https://i.imgur.com/zRET6e6.png' );

  var material = new THREE.MeshPhongMaterial({map: texture1, emissive: 0x0, specular: 0x878787, shininess: 3, receiveShadow : true});
  material.opacity = 0.85;
  material.transparent = true;
  material.blendSrcAlpha = 0.95;
  //material.receiveShadow = true;

  var phong = new THREE.MeshPhongMaterial({map: texture1, emissive: 0x0, specular: 0x878787, shininess: 3});
  phong.opacity = 0.85;
  phong.transparent = true;
  phong.blendSrcAlpha = 0.95;
  //phong.receiveShadow = true;

  var lambert = new THREE.MeshLambertMaterial({map: texture1, emissive: 0x0 });
  lambert.opacity = 0.85;
  lambert.transparent = true;
  lambert.blendSrcAlpha  = 0.95;
  //lambert.receiveShadow = true;
  
  
  var geometry1 = new THREE.PlaneGeometry(220, 20);
  var material1 = new THREE.MeshPhongMaterial({color: 0x12660d, emissive: 0x0, specular: 0x878787, shininess: 3, receiveShadow : true});
  material1.opacity = 1;
  material1.transparent = false;
  material1.blendSrcAlpha = 1;
  //material1.receiveShadow = true;

  var mesh1 = new THREE.Mesh(geometry1, material1);
  mesh1.position.set(0,-50,0);

  var mesh3 = new THREE.Mesh(geometry1, material1);
  mesh3.position.set(0,-150,0);

  var mesh4 = new THREE.Mesh(geometry1, material1);
  mesh4.position.set(0,+50,0);

  var texture2 = new THREE.TextureLoader().load( 'https://i.imgur.com/zRET6e6.png' );
  var map = new THREE.TextureLoader().load( 'https://i.imgur.com/gfDGI2L.png' );



  var material2 = new THREE.MeshPhongMaterial({map: texture2, emissive: 0x0, specular: 0x878787, shininess: 3, color: 0xe38c8c, receiveShadow : true});
  material2.opacity = 0.85;
  material2.transparent = true;
  material2.blendSrcAlpha = 0.95;
  //material2.receiveShadow = true;




  var mesh2 = new THREE.Mesh(geometry, material2);
  mesh2.position.set(0,-100,0)
 

  var mesh = new THREE.Mesh(geometry, material );
  mesh.position.set(0,50,0);

  mesh.add(mesh1);
  mesh.add(mesh3);
  mesh.add(mesh4);
  mesh.add(mesh2);

  mesh.receiveShadow = false;




  GameObject.call(this, position, mesh, material, lambert, phong, material2);
}
