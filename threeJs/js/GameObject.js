IN_WIREFRAME = false;
ILUMINATE = true;
PHONG_SWITCH = true;
window.addEventListener("keydown", function(key) {
    if (key.key == "A" || key.key == "a") IN_WIREFRAME = !IN_WIREFRAME;
    if (key.key == "L" || key.key == "l") ILUMINATE = !ILUMINATE;
    if (key.key == "G" || key.key == "g") PHONG_SWITCH = !PHONG_SWITCH;
});

function GameObject(position, mesh, basicMat, lambertMat, phongMat) {
    var self = this;
    
    this.mesh = mesh;
    this.mesh.position.set(position.x, position.y, position.z);
    this.wireframe = false;
    SCENE.add( this.mesh );
    this.mesh.castShadow = true;
    this.basicMat = basicMat;
    this.phongMat = phongMat;
    this.lambertMat = lambertMat;

    this.lightMat = PHONG_SWITCH ? this.phongMat : this.lambertMat;
    this.mesh.material = ILUMINATE ? this.lightMat : this.basicMat;

    this.isGameObject = true;

    this.target = this.mesh;
    
    this.switchWireFrame = function(key) {
        /*if (key.key === "A" || key.key === "a") {
            self.mesh.traverse(function (node) {
            if (node instanceof THREE.Mesh)
                node.material.wireframe = IN_WIREFRAME;
            });
            self.basicMat.wireframe = IN_WIREFRAME;
            self.lambertMat.wireframe = IN_WIREFRAME;
            self.phongMat.wireframe = IN_WIREFRAME;
        }*/
    }

    this.switchMat = function(key) {
        /*var targets;
        if (!(self.target instanceof Array)) targets = [self.target];
        else targets = self.target;
        
        targets.forEach(function(target) {
            switch (key.key) {
                case "L":
                case "l":
                    if (ILUMINATE) target.material = self.lightMat;
                    else target.material = self.basicMat;
                    break;
                case "G":
                case "g":
                    if (PHONG_SWITCH) self.lightMat = self.phongMat;
                    else self.lightMat = self.lambertMat;
                    if (ILUMINATE) target.material = self.lightMat;
                    break;
            }
        });*/
    }

    this.remove = function() {
        SCENE.remove(this.mesh);
        if (UPDATE.indexOf(this) >= 0) UPDATE.splice(UPDATE.indexOf(this), 1);
        if (SOLIDS.indexOf(this) >= 0) SOLIDS.splice(SOLIDS.indexOf(this), 1);
    }
    
    window.addEventListener("keydown", this.switchWireFrame);
    window.addEventListener("keydown", this.switchMat);
}
