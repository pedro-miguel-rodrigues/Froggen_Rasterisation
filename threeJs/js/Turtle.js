var turtle_material = new THREE.MeshBasicMaterial( { color: 0xe54545 } );
var turtle_phong = new THREE.MeshPhongMaterial({color: 0xe54545, emissive: 0x0, specular: 0x878787, shininess: 50});
var turtle_lambert = new THREE.MeshLambertMaterial({color: 0xe54545, emissive: 0x0});

function Turtle(position){
  var geometry = new THREE.TorusGeometry(3, 3, 20, 25);

  var material = turtle_phong;

  var mesh = new THREE.Mesh(geometry, material);

  var mesh1 = new THREE.Mesh(geometry, material);
  mesh1.position.set(-9,0,0);
  
  var mesh2 = new THREE.Mesh(geometry, material);
  mesh2.position.set(9,0,0);

  mesh.add(mesh1);
  mesh.add(mesh2);


  MovableObject.call(this, position, mesh, turtle_material, turtle_lambert, turtle_phong);
  SolidObject.call(this, position, mesh, new SphereBox(9), turtle_material, turtle_lambert, turtle_phong);

  this.speed = 0;
  this.direction = new THREE.Vector3(1,0,0);

  this.collide = function(other) {
      if (other instanceof Frog) {
          other.onATurtle = true;
      }
  };

  this.move = function(delta) {
        var dir = new THREE.Vector3(-1, 0, 0);
        this.mesh.position.add(dir.multiplyScalar(this.speed*delta));
    };

  this.update = function(delta) {
    this.speed = TURTLE_SPEED;
    if (this.mesh.position.x < -130) {
      this.mesh.position.x = 130;
    }
    this.move(delta);
  }

  this.speed = TURTLE_SPEED;
  this.direction = (new THREE.Vector3(1,0,0)).applyAxisAngle(new THREE.Vector3(0,0,1), 360*Math.random());
  UPDATE.push(this);
}
