var log_material = new THREE.MeshBasicMaterial({color: 0xfeffed});
var log_phong = new THREE.MeshPhongMaterial({color: 0xfeffed, emissive: 0x0, specular: 0x878787, shininess: 5});
var log_lambert = new THREE.MeshLambertMaterial({color: 0xfeffed, emissive: 0x0});

function Log(position) {
    var self = this;
    var geometry = new THREE.CylinderGeometry(8, 8, 40, 10, 10);
    var mesh = new THREE.Mesh(geometry, log_material);

    position.add(new THREE.Vector3(0,0,0));
    mesh.rotateZ(Math.PI/2);
    mesh.scale.set(1,1,1)

    mesh.up.set(0,0,1);

    MovableObject.call(this, position, mesh, log_material, log_material, log_material);
    SolidObject.call(this, position, mesh, new AxisAlignedBox(
    new THREE.Vector3(20, 5, 1),
    new THREE.Vector3(-20, -5, -1)
  ), log_material, log_material, log_material);
    

    this.move = function(delta) {
        var dir = new THREE.Vector3(1, 0, 0);
        this.mesh.position.add(dir.multiplyScalar(this.speed*delta));

        if (this.direction.z == 0) {
          var rotAxis = new THREE.Vector3(0,1,0);
          mesh.rotateOnAxis(rotAxis, -Math.PI*(this.speed/25)*delta)
        }
    };

    this.update = function(delta) {
        this.move(delta);
        this.speed = LOG_SPEED;
        if (this.mesh.position.x > 130) {
          this.mesh.position.x = -130;
        }
    };

    this.collide = function(other) {
        if (other instanceof Frog) {
            other.onALog = true;
        }
    };

    GameObject.call(this, position, mesh, log_material, log_phong, log_lambert);

    this.speed = LOG_SPEED;
    this.direction = new THREE.Vector3(1,0,0);
    UPDATE.push(this);

    //window.addEventListener("keydown", function(key) {
    //    if (key.key == "c" || key.key == "C") self.mesh.visible = !self.mesh.visible;
    //});
}