var black_bas = new THREE.MeshBasicMaterial({color: 0x0});
var black_phong = new THREE.MeshPhongMaterial({color: 0x0, emissive: 0x0, specular: 0x878787, shininess: 3});
var black_lamb = new THREE.MeshLambertMaterial({color: 0x101010, emissive: 0x0});


function make_cockpit_geometry() {
  var geo = new THREE.Geometry();
  geo.vertices.push(new THREE.Vector3(-6, -6, -3));
  geo.vertices.push(new THREE.Vector3(6, -6, -3));
  geo.vertices.push(new THREE.Vector3(-6, 6, -3));
  geo.vertices.push(new THREE.Vector3(6, 6, -3));
  geo.vertices.push(new THREE.Vector3(-6, -6, 1));
  geo.vertices.push(new THREE.Vector3(6, -6, 1));
  geo.vertices.push(new THREE.Vector3(-6, 6, 1));
  geo.vertices.push(new THREE.Vector3(6, 6, 1));
  
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

function Frog(position, direction) {
  'use strict';

  var self = this;
  var onALog = false;
  var onATurtle = false;

  this.midLap = false;
  this.speed = 0;

  var mesh = new THREE.Object3D();

  var phong = new THREE.MeshPhongMaterial({color: 0xc8f609, emissive: 0x0, specular: 0x878787, shininess: 80});
  var lambert = new THREE.MeshLambertMaterial({color: 0xc8f609, emissive: 0x0});
  // Add cockpit
  var geometry = make_cockpit_geometry();
  var material = phong;
  var cockmesh = new THREE.Mesh( geometry, material );
  cockmesh.position.set(0, 0, 3);
  mesh.add(cockmesh);

  // Add headlights
  geometry = make_headlight();
  var material2 = black_phong;
  var headlightmesh1 = new THREE.Mesh(geometry, material2);
  var headlightmesh2 = new THREE.Mesh(geometry, material2);
  headlightmesh1.position.set(1,6.1,3);
  headlightmesh2.position.set(-1,6.1,3); 
  mesh.add(headlightmesh1, headlightmesh2);

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

  this.extraParts = [headlightmesh1, headlightmesh2];

  MovableObject.call(this, position, mesh, material, lambert, phong);
  SolidObject.call(this, position, mesh, new SphereBox(6), material, lambert, phong);
  this.baseAcceleration = 100;
  this.direction = direction;
  this.potAngularSpd = 0;

  this.target = [cockmesh];

  this.target.material = ILUMINATE ? this.lightMat : this.basicMat;

  this.lightMat2 = PHONG_SWITCH ? black_phong : black_lamb;

  this.targets = [[-90, 0], [-70, 0], [-50, 0], [30, 0]];
  this.currentTarget = 0;

  // Movement
  this.keypress = function(key) {
    switch (key.key) {
      case "ArrowUp":
        if(LIVES > 0){
          mesh.position.y += 20;
        }
        break;

      case "ArrowDown":
        if(LIVES > 0){
          if(mesh.position.y > -90){
            mesh.position.y -= 20;
          }
        }
        break;

      case "ArrowLeft":
        if(LIVES > 0){
          if(mesh.position.x > -90){
            mesh.position.x -= 20;
          }
        }
        break;

      case "ArrowRight":
        if(LIVES > 0){
          if(mesh.position.x <= 90){
            mesh.position.x += 20;
          }
        }
        break;
    }
  }

  this.followPath = function(delta) {
    var direction = 1;
  if (this.mesh.position.x <= -110) {
    direction = 1;
  }

  else if (this.mesh.position.x >= 110 - 16) {
    direction = -1;
  }
  
    this.mesh.position.x += direction * delta * 10; 
  }
  

  this.update = function(delta) {
    this.angularSpd = this.potAngularSpd;
    if (STEREO_ON) this.followPath(delta);

    if(this.mesh.position.y > 10){
      if(this.onALog){
        var dir = new THREE.Vector3(1, 0, 0);
        this.mesh.position.add(dir.multiplyScalar(LOG_SPEED*delta));
        this.onALog = false;
      }
      else if(this.onATurtle){
        var dir = new THREE.Vector3(-1, 0, 0);
        this.mesh.position.add(dir.multiplyScalar(TURTLE_SPEED*delta));
        this.onATurtle = false;
      }
      else if(this.mesh.position.y > 90){
        SCORE++;
        document.getElementById("score").innerHTML = SCORE;
        PARTICLE_SYSTEM.reset();
        this.mesh.position.copy(INITIAL_POSITION);
      }
      else{
        console.log("Sploosh!");
        LIVES--;
        L_DSP.updateLives(LIVES);
        if (LIVES <= 0) {
          PAUSED = true;
          GAME_OVER = true;
          GO_TXT.mesh.visible = true;
        }
        this.speed = 0;
        this.mesh.position.copy(INITIAL_POSITION);
      }
    }

    this.speed = 0;

    var disp = this.move(delta);
    disp.divideScalar(20);
    if (this.isOutside()) {
      console.log("Sploosh!");
      LIVES--;
      L_DSP.updateLives(LIVES);
      if (LIVES <= 0) {
        PAUSED = true;
        GAME_OVER = true;
        GO_TXT.mesh.visible = true;
      }
      this.speed = 0;
      this.mesh.position.copy(INITIAL_POSITION);
    }

    this.light1.target.position.copy(this.mesh.position).add(this.direction.clone().multiplyScalar(100));
    this.light2.target.position.copy(this.mesh.position).add(this.direction.clone().multiplyScalar(100));
  };

  this.collide = function(other) {

    if (other instanceof Truck || other instanceof Car) {
      console.log("Sploosh!");

      LIVES--;
      L_DSP.updateLives(LIVES);

      if (LIVES <= 0) {
        PAUSED = true;
        GAME_OVER = true;
        GO_TXT.mesh.visible = true;
      }
      
      this.speed = 0;
      this.mesh.position.copy(INITIAL_POSITION);
    }

    if (other instanceof Butter) {
      var disp = this.direction.clone().multiplyScalar(Math.sign(this.speed));
      disp.divideScalar(20);
      this.speed = -Math.sign(this.speed) * 0.001;
       if (disp.x == 0 && disp.y == 0) disp = new THREE.Vector3(0.001,0,0);
      while (this.hasCollision(other)) {
        this.mesh.position.sub(disp);
        //console.log("Butter");
      }
    }
  };

  this.switchMat2 = function(key) {
    /*self.extraParts.forEach(function(part) {
      switch (key.key) {
          case "L":
          case "l":
              if (ILUMINATE) part.material = self.lightMat2;
              else part.material = black_bas;
              break;
          case "G":
          case "g":
              if (PHONG_SWITCH) self.lightMat2 = black_phong;
              else self.lightMat2 = black_lamb;
              if (ILUMINATE) part.material = self.lightMat2;
              break;
      }
   });*/
  };

  this.switchWireFrame2 = function(key) {
    /*if (key.key === "A" || key.key === "a") {
        black_bas.wireframe = IN_WIREFRAME;
        black_lamb.wireframe = IN_WIREFRAME;
        black_phong.wireframe = IN_WIREFRAME;
    }*/
}

  window.addEventListener("keydown", this.keypress);
  window.addEventListener("keyup", this.keyup);
  window.addEventListener("keydown", this.switchMat2);
  window.addEventListener("keydown", this.switchWireFrame2);

  window.addEventListener("keydown", function(key) {
    if (key.key == "H" || key.key == "h") {
      self.light1.visible = !self.light1.visible;
      self.light2.visible = !self.light2.visible;
    }
  });
}
