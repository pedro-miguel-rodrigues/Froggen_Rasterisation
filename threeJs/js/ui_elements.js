function PauseTxt() {
    
    var loader = new THREE.TextureLoader();
    loader.crossOrigin = "";

    var self = this;
    this.texture = loader.load('https://i.imgur.com/GrvgqnG.png');

    this.geometry = new THREE.PlaneGeometry(100,50);
    this.material = new THREE.MeshBasicMaterial({map: this.texture, transparent: true});
    this.mesh = new THREE.Mesh(this.geometry, this.material);
    this.mesh.visible = false;
    UI_SCENE.add(this.mesh);

    window.addEventListener('keydown', function(key) {
        if (GAME_OVER) return;
        if (key.key == "S" || key.key == "s") self.mesh.visible = !self.mesh.visible;
    });
}

function GameOverTxt() {

    var loader = new THREE.TextureLoader();
    loader.crossOrigin = "";

    var self = this;
    this.texture = loader.load('https://i.imgur.com/t2bsshE.png');

    this.geometry = new THREE.PlaneGeometry(200,50);
    this.material = new THREE.MeshBasicMaterial({map: this.texture, transparent: true});
    this.mesh = new THREE.Mesh(this.geometry, this.material);
    this.mesh.visible = false;
    UI_SCENE.add(this.mesh);

    this.mesh.visible = false;
}

function HitPoints() {
    var self = this;
    var frogMesh = FROG.mesh.clone();
    this.models = [
        frogMesh.clone(),
        frogMesh.clone(),
        frogMesh.clone(),
        frogMesh.clone(),
        frogMesh.clone()
    ];

    this.meshes = new THREE.Object3D();
    console.log(this.models);

    this.models.forEach(function (model, i) {
        model.position.set(-100 + 15 * i, 110, 0);
        self.meshes.add(model);
    });

    UI_SCENE.add(this.meshes);

    this.updateLives = function(l) {
        this.models.forEach(function(model, i) {
            model.visible = i<l;
        });
    }
}


function ScoreTxt() {
    /*var self = this;
    this.mesh = new THREE.Object3D();
    this.childMesh = null
    this.font = null;
    this.mesh.position.set(30, 3, 0);
    this.material = new THREE.MeshBasicMaterial({color: 0x4});
    this.mesh.visible = true;
    UI_SCENE.add(this.mesh);


    

    var loader = new THREE.FontLoader();
    loader.load('https://github.com/rollup/three-jsnext/blob/master/examples/fonts/helvetiker_regular.typeface.json', (font) => {
        var geometry = new THREE.TextGeometry('Score: 0', {
            font: font,
            size: 50,
        });
        var txt_mesh = new THREE.Mesh(geometry, self.material);

        self.mesh.add(txt_mesh);
        self.childMesh = txt_mesh;
        self.font = font;
    });

    this.updateScore = (score) => {
        self.mesh.remove(self.childMesh);
        var geometry = new THREE.TextGeometry('Score: ${score}', {
            font: self.font,
            size: 5,
        });
        var txt_mesh = new THREE.Mesh(geometry, self.material);

        self.mesh.add(txt_mesh);
        self.childMesh = txt_mesh;
        console.log(txt_mesh);
    };
    */
}

