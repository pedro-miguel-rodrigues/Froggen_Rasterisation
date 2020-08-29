var SCENE;
var CAMERA1, CAMERA2, CAMERA3;
var CAMERAS = [CAMERA1, CAMERA2, CAMERA3];
var CAMERA = CAMERA1;
var STEREO_CAMERA;
var CONTROLLER;

var HEAD_ANGLE = 0;
var HEAD_HEIGHT = 0;

var STEREO_ON = false;

var RENDERER;
var FROG; // Needed to make camera follow FROG.
//var ORANGE_SPEED = 10;
//var ORANGE_TIME = 2000;
var LOG_SPEED = 10;
var TRUCK_SPEED = 6;
var TURTLE_SPEED = 8;
var INITIAL_POSITION = new THREE.Vector3(0,-100,0);
var SUN;
var TABLE;
var TABLE2;
var SKYBOX;

var PARTICLE_SYSTEM;

var NODE;

var LIVES = 5;
var SCORE = 0;
var GAME_OVER = false;

var UI_SCENE;
var UI_CAM;
var P_TXT, GO_TXT, L_DSP, SCORE_TXT;

var UPDATE = [];
var SOLIDS = [];
var CLOCK = new THREE.Clock();

var PAUSED = false;
var fogColor;

var WIDTH = 240, HEIGHT = 240;

var texture5, billboards, mapHeight = new THREE.TextureLoader().load( "https://i.imgur.com/SDJi9bW.jpg" );

function initScene(){
  'use strict';

  SCENE = new THREE.Scene();

  fogColor = new THREE.Color(0xc9518b);
  //SCENE.background = fogColor;
  SCENE.fog = null;

  //updateOrangeFog(SCENE);
  updateButterFog(SCENE);
  
  
  CAMERA1 = new THREE.OrthographicCamera( WIDTH / - 2, WIDTH / 2, HEIGHT / 2, HEIGHT / - 2, 1, 1000  );
  RENDERER = new THREE.WebGLRenderer();
  RENDERER.autoClear = false;
  RENDERER.setSize( window.innerWidth, window.innerHeight );
  RENDERER.shadowMapEnabled = true; // CHANGE ME
  RENDERER.shadowMap.type = THREE.PCFSoftShadowMap;
  NODE = RENDERER.domElement;
  document.body.appendChild(RENDERER.domElement);
  CAMERA1.position.z = 200;

  CAMERA2 = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 5, 800);
  CAMERA3 = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 5, 800);
  CAMERA2.position.set(200, 200, 100);
  CAMERA2.up = new THREE.Vector3(0,0,1);
  CAMERA3.up = new THREE.Vector3(0,0,1);
  CAMERA3.position.set(0, 0, 10);
  CAMERA2.lookAt(new THREE.Vector3(0,0,0));

  CONTROLLER = new THREE.OrbitControls(CAMERA2);

  CONTROLLER.maxPolarAngle = Math.PI / 2 - 0.1;
  CONTROLLER.keyPanSpeed = 0;

  STEREO_CAMERA = new THREE.StereoCamera();
  STEREO_CAMERA.aspect = 0.5;

  CAMERAS = [CAMERA1, CAMERA2, CAMERA3];
  CAMERA = CAMERA1
}

function onResize(){
  'use strict';

  RENDERER.setSize(window.innerWidth, window.innerHeight);
  if (window.innerHeight > 0 && window.innerWidth > 0){
    var width = WIDTH, height = HEIGHT;
    var aspRatio = RENDERER.getSize().width/RENDERER.getSize().height;
    if (RENDERER.getSize().width > RENDERER.getSize().height){
      width = height*aspRatio;
    } else {
      height = width/aspRatio;
    }
    CAMERA1.left = width / -2;
    CAMERA1.right = width / 2;
    CAMERA1.top = height / 2;
    CAMERA1.bottom = height / - 2;
    CAMERA1.updateProjectionMatrix();
    UI_CAM.left = width / -2;
    UI_CAM.right = width / 2;
    UI_CAM.top = height / 2;
    UI_CAM.bottom = height / - 2;
    UI_CAM.updateProjectionMatrix();
    CAMERA2.aspect = aspRatio;
    CAMERA3.aspect = aspRatio;
    CAMERA2.updateProjectionMatrix();
    CAMERA3.updateProjectionMatrix();
  }
  RENDERER.render( SCENE, CAMERA );
}

function createTrack(){
  'use strict';

  function createTable(){
    'use strict';
    //lane 8
    new Log(new THREE.Vector3(-90, 80, -6));
    new Log(new THREE.Vector3(0, 80, -6));
    new Log(new THREE.Vector3(90, 80, -6));
    //lane 7
    new Turtle(new THREE.Vector3(-90, 60, -1));
    new Turtle(new THREE.Vector3(-30, 60, -1));
    new Turtle(new THREE.Vector3(50, 60, -1));
    new Turtle(new THREE.Vector3(110, 60, -1));
    //lane 6
    new Log(new THREE.Vector3(-120, 40, -6));
    new Log(new THREE.Vector3(-50, 40, -6));
    new Log(new THREE.Vector3(50, 40, -6));
    //lane 5
    new Turtle(new THREE.Vector3(-100, 20, -1));
    new Turtle(new THREE.Vector3(-20, 20, -1));
    new Turtle(new THREE.Vector3(60, 20, -1));
    new Turtle(new THREE.Vector3(100, 20, -1));
    //lane 4
    //lane 3
    //lane 2
    //lane 1
    new Truck(new THREE.Vector3(-100, -80, 0));
    new Truck(new THREE.Vector3(-20, -80, 0));
    new Truck(new THREE.Vector3(60, -80, 0));
    new Truck(new THREE.Vector3(105, -80, 0));


    new Truck(new THREE.Vector3(-110, -40, 0));
    new Truck(new THREE.Vector3(-30, -40, 0));
    new Truck(new THREE.Vector3(30, -40, 0));
    new Truck(new THREE.Vector3(95, -40, 0));


    new Car(new THREE.Vector3(-100, -60, 0));
    new Car(new THREE.Vector3(-20, -60, 0));
    new Car(new THREE.Vector3(60, -60, 0));
    new Car(new THREE.Vector3(105, -60, 0));

    new Car(new THREE.Vector3(-110, -20, 0));
    new Car(new THREE.Vector3(-30, -20, 0));
    new Car(new THREE.Vector3(30, -20, 0));
    new Car(new THREE.Vector3(95, -20, 0));

  }

  createTable();

  new Butter(new THREE.Vector3(0,0,5));


  new Candle(new THREE.Vector3(50, 50, 0));
  new Candle(new THREE.Vector3(-50, 50, 0));
  new Candle(new THREE.Vector3(50, -50, 0));
  new Candle(new THREE.Vector3(-50, -50, 0));
  new Candle(new THREE.Vector3(30, 0, 0));
  new Candle(new THREE.Vector3(-30, 100, 0));

    // Create the flags.
    UPDATE.push(new Flag(new THREE.Vector3(35,100,0)));
    UPDATE.push(new Flag(new THREE.Vector3(85,100,0)));
    UPDATE.push(new Flag(new THREE.Vector3(-35,100,0)));
    UPDATE.push(new Flag(new THREE.Vector3(-85,100,0)));

    PARTICLE_SYSTEM = new ParticleSystem();
    UPDATE.push(PARTICLE_SYSTEM);
}

function initializeMirrors(){
  var geom1 = new THREE.PlaneBufferGeometry(220, 220);
  new Mirror(SCENE, geom1, (mirror) => {
    mirror.position.x = 0;
    mirror.position.y = 0;
    mirror.position.z = -0.01;
  });
}

function gameStart() {
  'use strict';

  initializeMirrors();
  TABLE = new Table(RENDERER);
  //TABLE2 = new TableDown();
  createTrack();
  FROG = new Frog(INITIAL_POSITION, new THREE.Vector3(0,1,0));
  UPDATE.push(FROG);
  
  SUN = new THREE.DirectionalLight(0xf7e7c0, 1.2);
  SUN.position.set(200, 200, 10);
  SUN.castShadow = true;
  SUN.target = TABLE.mesh;
  SCENE.add(SUN);

  var loader = new THREE.TextureLoader();
  loader.crossOrigin = "";

  var flare1 = loader.load('https://i.imgur.com/A5CvDmW.png');
  var flare2 = loader.load('https://i.imgur.com/Oz4Q0xj.png');
  var flare3 = loader.load('https://i.imgur.com/oegv1OW.png');
  var shine1 = loader.load('https://i.imgur.com/qyaDuvc.png');


  var lensflare = new THREE.Lensflare();
  lensflare.addElement( new THREE.LensflareElement(shine1, 200, 0, new THREE.Color(0xffffff)) );
  lensflare.addElement( new THREE.LensflareElement(flare1, 100, 0.3, new THREE.Color(0xb5dae0)) );
  lensflare.addElement( new THREE.LensflareElement(flare2, 180, 0.5, new THREE.Color(0x58e2b9)) );
  lensflare.addElement( new THREE.LensflareElement(flare3, 140, 0.65, new THREE.Color(0xb5dae0)) );

  SUN.add(lensflare);

  SCENE.add(new THREE.AmbientLight(0x6985b2, 0.1));

  //skybox
  var geometry = new THREE.CubeGeometry(600,600,600);
  var cubeMaterials = [
            new THREE.MeshBasicMaterial({map: loader.load('https://i.imgur.com/HG0lUZG.png'), side: THREE.DoubleSide}),
            new THREE.MeshBasicMaterial({map: loader.load('https://i.imgur.com/82ydNmK.png'), side: THREE.DoubleSide}),
            new THREE.MeshBasicMaterial({map: loader.load('https://i.imgur.com/0px2qjW.png'), side: THREE.DoubleSide}),
            new THREE.MeshBasicMaterial({map: loader.load('https://upload.wikimedia.org/wikipedia/commons/6/68/Solid_black.png'), side: THREE.DoubleSide}),
            new THREE.MeshBasicMaterial({map: loader.load('https://i.imgur.com/JKAuUe2.png'), side: THREE.DoubleSide}),
            new THREE.MeshBasicMaterial({map: loader.load('https://i.imgur.com/Kw9hxw0.png'), side: THREE.DoubleSide}),
  ];

  var cubeMaterial = new THREE.MeshFaceMaterial(cubeMaterials);
  SKYBOX = new THREE.Mesh(geometry, cubeMaterial);
  SKYBOX.rotateX(Math.PI/2);
  SKYBOX.position.set(0,0,0);
  SCENE.add(SKYBOX);
  SCENE.background = SKYBOX;
}

function updateFollowCamera(delta) {
  updateMovieCamera(delta);
  if (CAMERA != CAMERA3 && !STEREO_ON) return;
  var pos = new THREE.Vector3(FROG.mesh.position.x, FROG.mesh.position.y, FROG.mesh.position.z);
  var dir = FROG.direction.clone();
  dir.applyAxisAngle(new THREE.Vector3(0,0,1), HEAD_ANGLE);
  dir.multiplyScalar(80);
  var pos2 = pos.clone();
  pos2.sub(dir);
  pos2.z = 30 + HEAD_HEIGHT;
  CAMERA3.position.lerp(pos2, 3 * delta);
  CAMERA3.lookAt(pos);
}

function setupUI() {
  UI_SCENE = new THREE.Scene();
  UI_CAM = new THREE.OrthographicCamera( WIDTH / - 2, WIDTH / 2, HEIGHT / 2, HEIGHT / - 2, 1, 1000  );
  UI_CAM.position.set(0,0,400);
  P_TXT = new PauseTxt();
  GO_TXT = new GameOverTxt();
  L_DSP = new HitPoints();
  SCORE_TXT = new ScoreTxt();

  var light = new THREE.DirectionalLight(0xffffff, 0.5);
  light.position.set(0,0,100);

  light.castShadow = true;




  UI_SCENE.add(light);
  
}


function animate() {
  'use strict';

  var delta = CLOCK.getDelta();
  if (PAUSED) delta = 0;

  // Collision checking.
  var unchecked = [].concat(SOLIDS);
  SOLIDS.forEach(function() {
    var obj = unchecked.pop();
    unchecked.forEach(function(other) {
      if (obj.hasCollision(other)) {
        obj.collide(other);
        other.collide(obj);
      }
    });
  });

  // Update each object
  UPDATE.forEach(function(ob) {
    if (ob.update) ob.update(delta);
  });

  updateFollowCamera(delta);
  STEREO_CAMERA.update(CAMERA3);

  // Render
  if (!STEREO_ON) {
    RENDERER.clear();
    RENDERER.render(SCENE, CAMERA);
    RENDERER.clearDepth();
    RENDERER.render(UI_SCENE, UI_CAM);
  }
  else
    stereoRender();

  CONTROLLER.update();
  requestAnimationFrame(animate);
}

window.addEventListener("resize", onResize);
window.addEventListener("keydown", function(key) {
  switch (key.key) {
    case "1":
      CAMERA = CAMERA1;
      //deactivateMovie();
      break;

    case "2":
      CAMERA = CAMERA2;
      //deactivateMovie();
      break;

    case "3":
      CAMERA = CAMERA3;
      //deactivateMovie();
      break

    case "N":
    case "n":
      SUN.visible = !SUN.visible;
      break;

    case "S":
    case "s":
      if (!GAME_OVER) PAUSED = !PAUSED;
      break;
      
    case "F":
    case "f":
        if(SCENE.fog != null){
            SCENE.fog = null;
        }
        else{
            SCENE.fog = new THREE.FogExp2(fogColor,0.0095);
        }
        updateButterFog(SCENE);
        break;

    case "L":
    case "l":
      TABLE.mesh.receiveShadow = !TABLE.mesh.receiveShadow;
      //TABLE2.mesh.receiveShadow = !TABLE2.mesh.receiveShadow;
      break;
    case "R":
    case "r":
      restartGame();
      break; 
        
  }



});

function stereoRender() {
  var size = RENDERER.getSize();
  RENDERER.setScissorTest(true);

  SCENE.updateMatrixWorld();
  STEREO_CAMERA.update(CAMERA3);

  RENDERER.clear();

  RENDERER.setScissor( 0, 0, size.width / 2, size.height );
  RENDERER.setViewport( 0, 0, size.width / 2, size.height );
  RENDERER.render( SCENE, STEREO_CAMERA.cameraL );
  
  RENDERER.setScissor( size.width / 2, 0, size.width / 2, size.height );
  RENDERER.setViewport( size.width / 2, 0, size.width / 2, size.height );
  RENDERER.render( SCENE, STEREO_CAMERA.cameraR );
  
  RENDERER.setScissorTest(false);
  RENDERER.setViewport(0,0, size.width, size.height);
}

function restartGame() {
  // Reset Global Vars.
  CAMERA = CAMERA1;
  LOG_SPEED = 10;
  TURTLE_SPEED = 8;
  TRUCK_SPEED = 6;
  INITIAL_POSITION = new THREE.Vector3(0,-100,0);
  LIVES = 5;
  GAME_OVER = false;
  
  SCORE = 0;
  document.getElementById("score").innerHTML = SCORE;
  
  UPDATE = [];
  SOLIDS = [];
  CLOCK = new THREE.Clock();
  
  PAUSED = false;
  
  WIDTH = 240, HEIGHT = 240;

  IN_WIREFRAME = false;
  ILUMINATE = true;
  PHONG_SWITCH = true;

  // Reset the Renderer
  RENDERER.dispose();
  document.body.removeChild(NODE);

  // Re-initialize the game.
  initScene();
  gameStart();
  setupUI();
  onResize();
}

window.addEventListener("keydown", function(key) {
  if (!GAME_OVER) return;
  if (key.key == "R" || key.key == "r") {
    restartGame();
  }
});

setInterval(function() {
  if (PAUSED) return;
  TRUCK_SPEED = TRUCK_SPEED + (80 - TRUCK_SPEED) * 0.05;
  LOG_SPEED = LOG_SPEED + (90 - LOG_SPEED) * 0.05;
  TURTLE_SPEED = TURTLE_SPEED + (100 - TURTLE_SPEED) * 0.05;
}, 10000);


window.addEventListener("deviceorientation", function(event) {
  HEAD_ANGLE = (event.alpha / 360) * 2 * Math.PI;
  HEAD_HEIGHT = Math.max((event.gamma / 360) * 200, - 20);
}, true);

initScene();
gameStart();
setupUI();
onResize();
animate();

function setStereo() {
  document.documentElement.webkitRequestFullScreen();
  window.screen.orientation.lock("landscape-primary");
}