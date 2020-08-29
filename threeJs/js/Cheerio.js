var cheerio_material = new THREE.MeshBasicMaterial( { color: 0xfffb44 } );
var cheerio_phong = new THREE.MeshPhongMaterial({color: 0xfffb44, emissive: 0x0, specular: 0x878787, shininess: 50});
var cheerio_lambert = new THREE.MeshLambertMaterial({color: 0xfffb44, emissive: 0x0});

function Cheerio(position){
  var geometry = new THREE.TorusGeometry(1.25, 0.75, 5, 7);
  var material = cheerio_material;
  var mesh = new THREE.Mesh( geometry, material );
  MovableObject.call(this, position, mesh, cheerio_material, cheerio_lambert, cheerio_phong);
  SolidObject.call(this, position, mesh, new SphereBox(2), cheerio_material, cheerio_lambert, cheerio_phong);

  this.speed = 0;
  this.direction = new THREE.Vector3(1,0,0);

  this.collide = function(other) {
    if (other instanceof Orange) return; // Disable collision with oranges for performance.
    var disp = other.mesh.position.clone().sub(this.mesh.position).normalize().divideScalar(40);
    disp.z = 0;
    if (disp.x == 0 && disp.y == 0) disp = new THREE.Vector3(1,0,0);
    while (this.hasCollision(other)) {
      this.mesh.position.sub(disp);
    }
    if (!other instanceof Cheerio) this.direction = disp.normalize().negate();
    if (other instanceof Car || other instanceof Orange) this.direction = other.direction.clone().multiplyScalar(Math.sign(other.speed));
    if (other instanceof Butter) this.speed = 0;
    else this.speed = Math.max(this.speed / 2, Math.abs(other.speed / 2));
  };

  this.update = function(delta) {
    if (this.isOutside()) {
      this.speed = 120;
      this.direction = new THREE.Vector3(0,0,-1);
    }

    if (this.mesh.position.z < -50) {
      this.remove();
    }
    this.move(delta);
  }

  UPDATE.push(this);
}
