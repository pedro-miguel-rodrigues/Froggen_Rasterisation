var loader = new THREE.TextureLoader();
loader.crossOrigin = "";

var flag_texture = loader.load('https://i.imgur.com/Y48TlUj.png');
var flag_material = new THREE.MeshPhongMaterial({map: flag_texture, transparent: true});


function Flag(position) {
    var self = this;
    var geometry = new THREE.PlaneGeometry(10, 10);
    var mesh = new THREE.Mesh(geometry, flag_material);

    position.add(new THREE.Vector3(0,0,5));

    mesh.up.set(0,0,1);
    mesh.rotateX(Math.PI / 2);

    GameObject.call(this, position, mesh, flag_material, flag_material, flag_material);
    this.mesh.castShadow = false;

    this.update = (delta) => {
        // Rotate to camera.
        var to = CAMERA.position.clone();
        to.setZ(5);

        this.mesh.lookAt(to);
        //console.log(to);
    };
}