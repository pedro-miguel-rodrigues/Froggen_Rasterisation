var ISMOVIE = false;
var CAMERATIME = 1000;
var LAST = 1;
var MOVIECAM;
var LASTFOCUS;

setTimeout(nextCamera,  Math.random()*CAMERATIME + CAMERATIME);

function deactivateMovie(){
  ISMOVIE = false;
}

function activateMovie(){
  ISMOVIE = true;
  MOVIECAM = movieCamera1();
  CAMERA = MOVIECAM;
  LAST = 1;
}

function nextCamera(){
  setTimeout(nextCamera,  Math.random()*CAMERATIME + CAMERATIME);
  if (ISMOVIE){
    var newCam = LAST;
    while (newCam == LAST){
      newCam = Math.floor((Math.random() * 5) + 1)
    }
    LAST = newCam;
    switch (newCam) {
      case 1:
        MOVIECAM = movieCamera1(); // In front overhead
        console.log("changing to movie camera 1");
        break;
      case 2:
        MOVIECAM = movieCamera2(); // Follow right wheel
        console.log("changing to movie camera 2");
        break;
      case 3:
        MOVIECAM = movieCamera3(); // Follow left wheel
        console.log("changing to movie camera 3");
        break;
      case 4:
        MOVIECAM = movieCamera4(); // Car Passes by camera
        console.log("changing to movie camera 4");
      case 5:
        MOVIECAM = movieCamera5(); // Follow ahead
        console.log("changing to movie camera 5");
      }
    CAMERA = MOVIECAM;
    console.log("last is"+LAST)
  }
}

function updateMovieCamera(delta){
  if (((LAST == 1) || (LAST == 4)) && ISMOVIE){
    var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
    MOVIECAM.lookAt(pos);
  }
  if ((LAST == 2) && ISMOVIE){
    var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
    var dir = FROG.direction.clone();
    var deltapos = new THREE.Vector3(dir.x,dir.y + 20,0);
    deltapos.multiplyScalar(5);
    var pos2 = pos.clone();
    pos2.add(deltapos);
    pos2.sub(dir.multiplyScalar(9));
    pos2.z = 4;
    MOVIECAM.position.set(pos2.x, pos2.y, pos2.z);
    dir.multiplyScalar(100);
    var pos3 = pos.clone();
    pos3.add(dir);
    MOVIECAM.up = new THREE.Vector3(0,0,1);
    pos3.lerp(LASTFOCUS, 10*delta);
    MOVIECAM.lookAt(pos3);
    LASTFOCUS = pos3.clone();
  }
  if ((LAST == 3) && ISMOVIE){
    var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
    var dir = FROG.direction.clone();
    var deltapos = new THREE.Vector3(dir.x,dir.y + 20,0);
    deltapos.multiplyScalar(5);
    var pos2 = pos.clone();
    pos2.add(deltapos);
    pos2.sub(dir.multiplyScalar(9));
    pos2.z = 4;
    MOVIECAM.position.set(pos2.x, pos2.y, pos2.z);
    dir.multiplyScalar(100);
    var pos3 = pos.clone();
    pos3.add(dir);
    MOVIECAM.up = new THREE.Vector3(0,0,1);
    pos3.lerp(LASTFOCUS, 10*delta);
    MOVIECAM.lookAt(pos3);
    LASTFOCUS = pos3.clone();
  }

  if ((LAST == 5) && ISMOVIE){
    var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
    var dir = FROG.direction.clone();
    dir.multiplyScalar(30);
    var pos2 = pos.clone();
    pos2.add(dir);
    pos2.z = 40;
    MOVIECAM.position.lerp(pos2, 3 * delta);
    MOVIECAM.lookAt(pos);
    MOVIECAM.up = new THREE.Vector3(0,0,1);
  }
  /*
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  dir.multiplyScalar(80);
  var pos2 = pos.clone();
  pos2.sub(dir);
  pos2.z = 70
  CAMERA3.position.lerp(pos2, 3 * delta);
  CAMERA3.lookAt(pos);*/
}

function  movieCamera1(){
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 5, 800);
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  dir.multiplyScalar(80);
  var pos2 = pos.clone();
  pos2.add(dir);
  pos2.z = 70;
  camera.position.set(pos2.x, pos2.y, pos2.z);
  camera.lookAt(pos);
  camera.up = new THREE.Vector3(0,0,1);
  return camera;
}

function  movieCamera2(){
  camera = new THREE.PerspectiveCamera(20, window.innerWidth / window.innerHeight, 2, 800);
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  var deltapos = new THREE.Vector3(dir.x,dir.y + 20,0);
  deltapos.multiplyScalar(5);
  var pos2 = pos.clone();
  pos2.add(deltapos);
  pos2.sub(dir.multiplyScalar(9));
  pos2.z = 4;
  camera.position.set(pos2.x, pos2.y, pos2.z);
  dir.multiplyScalar(100);
  var pos3 = pos.clone();
  pos3.add(dir);
  camera.up = new THREE.Vector3(0,0,1);
  camera.lookAt(pos3);
  LASTFOCUS = pos3.clone();
  return camera;
}

function  movieCamera3(){
  camera = new THREE.PerspectiveCamera(20, window.innerWidth / window.innerHeight, 2, 800);
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  var deltapos = new THREE.Vector3(dir.x,dir.y + 20,0);
  deltapos.multiplyScalar(5);
  var pos2 = pos.clone();
  pos2.add(deltapos);
  pos2.sub(dir.multiplyScalar(9));
  pos2.z = 4;
  camera.position.set(pos2.x, pos2.y, pos2.z);
  dir.multiplyScalar(100);
  var pos3 = pos.clone();
  pos3.add(dir);
  camera.up = new THREE.Vector3(0,0,1);
  camera.lookAt(pos3);
  LASTFOCUS = pos3.clone();
  return camera;
}

function  movieCamera4(){
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 5, 800);
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  dir.multiplyScalar(60);
  var pos2 = pos.clone();
  pos2.add(dir);
  pos2.z = 4
  camera.position.set(pos2.x, pos2.y, pos2.z);
  camera.lookAt(pos);
  camera.up = new THREE.Vector3(0,0,1);
  return camera;
}

function  movieCamera5(){
  camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 5, 800);
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  dir.multiplyScalar(30);
  var pos2 = pos.clone();
  pos2.add(dir);
  pos2.z = 40;
  camera.position.set(pos2.x, pos2.y, pos2.z);
  camera.lookAt(pos);
  camera.up = new THREE.Vector3(0,0,1);
  return camera;
}
