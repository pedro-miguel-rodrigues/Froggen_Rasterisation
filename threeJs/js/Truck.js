var black_bas = new THREE.MeshBasicMaterial({color: 0x0});
var black_phong = new THREE.MeshPhongMaterial({color: 0x0, emissive: 0x0, specular: 0x878787, shininess: 3});
var black_lamb = new THREE.MeshLambertMaterial({color: 0x101010, emissive: 0x0});

function make_cockpit_geometry() {
  var geo = new THREE.Geometry();
  geo.vertices.push(new THREE.Vector3(-3, -3, -3));
  geo.vertices.push(new THREE.Vector3(3, -3, -3));
  geo.vertices.push(new THREE.Vector3(-3, 3, -3));
  geo.vertices.push(new THREE.Vector3(3, 3, -3));
  geo.vertices.push(new THREE.Vector3(-2, -2, 3));
  geo.vertices.push(new THREE.Vector3(2, -2, 3));
  geo.vertices.push(new THREE.Vector3(-2, 2, 3));
  geo.vertices.push(new THREE.Vector3(2, 2, 3));
  
  geo.faces.push(new THREE.Face3(0,3,2));
  geo.faces.push(new THREE.Face3(1,3,0));
  geo.faces.push(new THREE.Face3(4,7,6));
  geo.faces.push(new THREE.Face3(4,5,7));
  geo.faces.push(new THREE.Face3(4,2,0));
  geo.faces.push(new THREE.Face3(2,4,6));
  geo.faces.push(new THREE.Face3(6,3,2));
  geo.faces.push(new THREE.Face3(3,6,7));
  geo.faces.push(new THREE.Face3(1,7,5));
  geo.faces.push(new THREE.Face3(7,1,3));
  geo.faces.push(new THREE.Face3(1,4,0));
  geo.faces.push(new THREE.Face3(4,1,5));

  geo.verticesNeedUpdate = true;
  geo.elementsNeedUpdate = true;

  geo.computeFaceNormals();
  geo.computeFlatVertexNormals();

  return geo;
}

function make_nose_geometry() {
  var geo = new THREE.Geometry();
  geo.vertices.push(new THREE.Vector3(-2,-3,-2));
  geo.vertices.push(new THREE.Vector3(2,-3,-2));
  geo.vertices.push(new THREE.Vector3(-2,3,-2));
  geo.vertices.push(new THREE.Vector3(2,3,-2));
  geo.vertices.push(new THREE.Vector3(-1.5,-4,2));
  geo.vertices.push(new THREE.Vector3(1.5,-4,2));
  geo.vertices.push(new THREE.Vector3(-1.5,2,2));
  geo.vertices.push(new THREE.Vector3(1.5,2,2));

  geo.faces.push(new THREE.Face3(0,3,2));
  geo.faces.push(new THREE.Face3(1,3,0));
  geo.faces.push(new THREE.Face3(4,7,6));
  geo.faces.push(new THREE.Face3(4,5,7));
  geo.faces.push(new THREE.Face3(4,2,0));
  geo.faces.push(new THREE.Face3(2,4,6));
  geo.faces.push(new THREE.Face3(6,3,2));
  geo.faces.push(new THREE.Face3(3,6,7));
  geo.faces.push(new THREE.Face3(1,7,5));
  geo.faces.push(new THREE.Face3(7,1,3));
  geo.faces.push(new THREE.Face3(1,4,0));
  geo.faces.push(new THREE.Face3(4,1,5));

  geo.verticesNeedUpdate = true;
  geo.elementsNeedUpdate = true;

  geo.computeFaceNormals();
  geo.computeFlatVertexNormals();

  return geo;
}

function make_headlight() {
  var geo = new THREE.Geometry();
  geo.vertices.push(new THREE.Vector3(0,0,0));
  geo.vertices.push(new THREE.Vector3(0.5,0.5,0.5));
  geo.vertices.push(new THREE.Vector3(0.5,0.5,-0.5));
  geo.vertices.push(new THREE.Vector3(-0.5,0.5,0.5));
  geo.vertices.push(new THREE.Vector3(-0.5,0.5,-0.5));

  geo.faces.push(new THREE.Face3(1,2,4));
  geo.faces.push(new THREE.Face3(1,4,3));
  geo.faces.push(new THREE.Face3(1,0,2));
  geo.faces.push(new THREE.Face3(0,4,2));
  geo.faces.push(new THREE.Face3(3,4,0));
  geo.faces.push(new THREE.Face3(0,1,3));

  geo.computeFaceNormals();

  return geo;
}

function Truck(position) {
  'use strict';

  var self = this;

  this.addBackWheel = function(x, y, z, mesh) {
    var geometry = new THREE.TorusGeometry( 1.25, 0.75, 6, 15);
    //var material = new THREE.MeshBasicMaterial( { color: 0x000000, wireframe: false } );
    var wheelmesh = new THREE.Mesh( geometry, black_phong );
    wheelmesh.rotateY(Math.PI/2);
    wheelmesh.position.set(x, y, z);
    mesh.add(wheelmesh);

    return wheelmesh;
  }

  this.addFrontWheel = function(x, y, z, mesh) {
    var geometry = new THREE.TorusGeometry( 0.5, 0.5, 6, 15);
    //var material = new THREE.MeshBasicMaterial( { color: 0x000000, wireframe: false } );
    var wheelmesh = new THREE.Mesh( geometry, black_phong );
    wheelmesh.rotateY(Math.PI/2);
    wheelmesh.position.set(x, y, z);
    mesh.add(wheelmesh);

    return wheelmesh;
  }

  var mesh = new THREE.Object3D();

  var phong = new THREE.MeshPhongMaterial({color: 0x615656, emissive: 0x0, specular: 0x878787, shininess: 80});
  var lambert = new THREE.MeshLambertMaterial({color: 0x615656, emissive: 0x0});

  // Add cockpit
  var geometry = make_cockpit_geometry(); //new THREE.BoxGeometry( 6, 6, 6 );
  var material = phong;
  var cockmesh = new THREE.Mesh( geometry, material );
  cockmesh.position.set(0, -3, 4);
  mesh.add(cockmesh);

  // Add nose
  geometry = make_nose_geometry(); //new THREE.BoxGeometry( 4, 6, 4 );
  var nosemesh = new THREE.Mesh( geometry, material);
  nosemesh.position.set(0, 3, 3);
  mesh.add(nosemesh);

  // Add back wheels
  var wheel1 = this.addBackWheel(-3.75,-3, 1.625, mesh);
  var wheel2 = this.addBackWheel( 3.75,-3, 1.625, mesh);

  // Add front wheels
  var wheel3 = this.addFrontWheel(-2.5, 4, 0.75, mesh);
  var wheel4 = this.addFrontWheel( 2.5, 4, 0.75, mesh);

  // Add headlights
  geometry = make_headlight();
  var material2 = black_phong;
  var headlightmesh1 = new THREE.Mesh(geometry, material2);
  var headlightmesh2 = new THREE.Mesh(geometry, material2);
  headlightmesh1.position.set(1,5.3,3);
  headlightmesh2.position.set(-1,5.3,3); 
  //mesh.add(headlightmesh1, headlightmesh2);

  this.light1 = new THREE.SpotLight(0xfffffffff, 1, 100, Math.PI/6, 0.7, 1);
  this.light1.position.set(1, 0.5, 0);
  this.light1.shadowCameraNear = 0.01;
  this.light1.castShadow = true;
  this.light1.shadowDarkness = 0.5;
  this.light2 = this.light1.clone();
  this.light2.position.set(-1, 0.5, 0)

  headlightmesh1.add(this.light1);
  headlightmesh2.add(this.light2);

  SCENE.add(this.light1.target, this.light2.target);

  this.extraParts = [headlightmesh1, headlightmesh2, wheel1, wheel2, wheel3, wheel4];

  MovableObject.call(this, position, mesh, material, lambert, phong);
  SolidObject.call(this, position, mesh, new SphereBox(6), material, lambert, phong);
  this.baseAcceleration = 100;
  this.potAngularSpd = 0;

  this.target = [cockmesh, nosemesh];

  this.target.material = ILUMINATE ? this.lightMat : this.basicMat;

  this.lightMat2 = PHONG_SWITCH ? black_phong : black_lamb;

  this.targets = [[60, 60], [-60, 60], [-60, -60], [60, -60]];
  this.currentTarget = 0;
  mesh.rotateZ(-Math.PI/2);


  this.move = function(delta) {
      var dir = new THREE.Vector3(1, 0, 0);
      this.mesh.position.add(dir.multiplyScalar(this.speed*delta));
    };

  this.update = function(delta) {
      this.move(delta);
      this.speed = TRUCK_SPEED;
      if (this.mesh.position.x > 130) {
        this.mesh.position.x = -130;
      }
  };

  this.speed = TRUCK_SPEED;
  this.direction = new THREE.Vector3(1,0,0);
  UPDATE.push(this);

  window.addEventListener("keydown", function(key) {
    if (key.key == "H" || key.key == "h") {
      self.light1.visible = !self.light1.visible;
      self.light2.visible = !self.light2.visible;
    }
  });
}
