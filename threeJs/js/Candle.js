var candle_material = new THREE.MeshBasicMaterial({color: 0xfeffed});
var candle_phong = new THREE.MeshPhongMaterial({color: 0xfeffed, emissive: 0x0, specular: 0x878787, shininess: 5});
var candle_lambert = new THREE.MeshLambertMaterial({color: 0xfeffed, emissive: 0x0});

function Candle(position) {
    var self = this;
    var geometry = new THREE.CylinderGeometry(1, 1, 5, 4, 2);
    var mesh = new THREE.Mesh(geometry, candle_material);

    position.add(new THREE.Vector3(0,0,2));

    mesh.up.set(0,0,1);
    mesh.rotateX(Math.PI / 2);

    var light = new THREE.PointLight(0xffe396, 1, 100, 1);
    light.position.set(0,10.5,0);
    light.shadowCameraNear = 0.01;
    light.castShadow = true;
    light.shadowDarkness = 0.5;
    mesh.add(light);

    GameObject.call(this, position, mesh, candle_material, candle_lambert, candle_phong);

    window.addEventListener("keydown", function(key) {
        if (key.key == "c" || key.key == "C") self.mesh.visible = !self.mesh.visible;
    });
}