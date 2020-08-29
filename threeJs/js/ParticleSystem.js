var loader = new THREE.TextureLoader();
loader.crossOrigin = "";

var particle_texture = loader.load('https://i.imgur.com/pkun8i0.png');
var particle_material = new THREE.MeshPhongMaterial({map: particle_texture, emissive: 0x0, specular: 0x878787, shininess: 5, transparent: true, color: 0xffbb32, depthWrite: false});


function ParticleSystem() {
    var self = this;
    var parentMesh = new THREE.Object3D();
    var zz = new THREE.Vector3(0,0,1);
    this.particleList = []

    this.createParticle = (position, initialAlpha, velocity, acceleration, alphaDecay) => {
        var geometry = new THREE.PlaneGeometry(4, 4);
        var mesh = new THREE.Mesh(geometry, particle_material);
        mesh.position.copy(position);
        mesh.up.set(0,0,1);

        this.particleList.push({
            initialPosition: position.clone(),
            initialAlpha: initialAlpha,
            alpha: 0.0,
            initialVelocity: velocity.clone(),
            velocity: velocity,
            acceleration: acceleration,
            alphaDecay: alphaDecay,
            mesh: mesh
        });

        parentMesh.add(mesh);
    };

    for (var i = 0; i < 100; i++) {
        // Create Particles
        this.createParticle(new THREE.Vector3(0, 0, 10),
            1.0,
            new THREE.Vector3(10, 0, 10).applyAxisAngle(zz, Math.random() * Math.PI * 2),
            new THREE.Vector3(0, 0, -20),
            0.5);
    }

    GameObject.call(this, new THREE.Vector3(0,0,0), parentMesh, flag_material, flag_material, flag_material);
    this.mesh.castShadow = false;

    this.update = (delta) => {
        this.particleList.forEach((particle) => {
            particle.mesh.position.add(particle.velocity.clone().multiplyScalar(delta));
            particle.velocity.add(particle.acceleration.clone().multiplyScalar(delta));
            particle.alpha -= particle.alphaDecay * delta;

            particle.mesh.lookAt(CAMERA.position);

            particle.mesh.material.opacity = particle.alpha;
        });
    };

    this.reset = () => {
        this.particleList.forEach((particle) => {
            particle.mesh.position.copy(particle.initialPosition);
            particle.velocity.copy(particle.initialVelocity);
            particle.alpha = particle.initialAlpha;
        })
    }
    
}