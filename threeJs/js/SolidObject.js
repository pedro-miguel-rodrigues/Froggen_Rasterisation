function SolidObject(position, mesh, boundingBox, basicMat, lambertMat, phongMat) {
    this.boundingBox = boundingBox;
    SOLIDS.push(this);

    if (!this.isGameObject) GameObject.call(this, position, mesh, basicMat, lambertMat, phongMat);

    this.hasCollision = function(other) {
        if (this.boundingBox.type == "AxisAlignedBox"  && other.boundingBox.type == "AxisAlignedBox") {
            selfMax = new THREE.Vector3(this.mesh.position.x + this.boundingBox.maxP.x,
                                        this.mesh.position.y + this.boundingBox.maxP.y,
                                        this.mesh.position.z + this.boundingBox.maxP.z);

            selfMin = new THREE.Vector3(this.mesh.position.x + this.boundingBox.minP.x,
                                        this.mesh.position.y + this.boundingBox.minP.y,
                                        this.mesh.position.z + this.boundingBox.minP.z);

            otherMax = new THREE.Vector3(other.mesh.position.x + other.boundingBox.maxP.x,
                                         other.mesh.position.y + other.boundingBox.maxP.y,
                                         other.mesh.position.z + other.boundingBox.maxP.z);

            otherMin = new THREE.Vector3(other.mesh.position.x + other.boundingBox.minP.x,
                                         other.mesh.position.y + other.boundingBox.minP.y,
                                         other.mesh.position.z + other.boundingBox.minP.z);

            return (selfMax.x >= otherMin.x && selfMax.y >= otherMin.y && selfMax.z >= otherMin.z  &&
                    selfMin.x <= otherMax.x && selfMin.y <= otherMax.y && selfMin.z <= otherMax.z) ||
                   (otherMax.x >= selfMin.x && otherMax.y >= selfMin.y && otherMax.z >= selfMin.z  &&
                    otherMin.x <= selfMax.x && otherMin.y <= selfMax.y && otherMin.z <= selfMax.z);
        }

        if (this.boundingBox.type == "AxisAlignedBox"  && other.boundingBox.type == "SphereBox") {
            selfMax = new THREE.Vector3(this.mesh.position.x + this.boundingBox.maxP.x,
                                        this.mesh.position.y + this.boundingBox.maxP.y,
                                        this.mesh.position.z + this.boundingBox.maxP.z);

            selfMin = new THREE.Vector3(this.mesh.position.x + this.boundingBox.minP.x,
                                        this.mesh.position.y + this.boundingBox.minP.y,
                                        this.mesh.position.z + this.boundingBox.minP.z);

            otherMax = new THREE.Vector3(other.mesh.position.x + other.boundingBox.radius,
                                         other.mesh.position.y + other.boundingBox.radius,
                                         other.mesh.position.z + other.boundingBox.radius);

            otherMin = new THREE.Vector3(other.mesh.position.x - other.boundingBox.radius,
                                         other.mesh.position.y - other.boundingBox.radius,
                                         other.mesh.position.z - other.boundingBox.radius);

            return (selfMax.x >= otherMin.x && selfMax.y >= otherMin.y && selfMax.z >= otherMin.z  &&
                    selfMin.x <= otherMax.x && selfMin.y <= otherMax.y && selfMin.z <= otherMax.z) ||
                    (otherMax.x >= selfMin.x && otherMax.y >= selfMin.y && otherMax.z >= selfMin.z  &&
                    otherMin.x <= selfMax.x && otherMin.y <= selfMax.y && otherMin.z <= selfMax.z);
        }

        if (this.boundingBox.type == "SphereBox"  && other.boundingBox.type == "AxisAlignedBox") {
            selfMax = new THREE.Vector3(this.mesh.position.x + this.boundingBox.radius,
                                        this.mesh.position.y + this.boundingBox.radius,
                                        this.mesh.position.z + this.boundingBox.radius);

            selfMin = new THREE.Vector3(this.mesh.position.x - this.boundingBox.radius,
                                        this.mesh.position.y - this.boundingBox.radius,
                                        this.mesh.position.z - this.boundingBox.radius);

            otherMax = new THREE.Vector3(other.mesh.position.x + other.boundingBox.maxP.x,
                                         other.mesh.position.y + other.boundingBox.maxP.y,
                                         other.mesh.position.z + other.boundingBox.maxP.z);

            otherMin = new THREE.Vector3(other.mesh.position.x + other.boundingBox.minP.x,
                                         other.mesh.position.y + other.boundingBox.minP.y,
                                         other.mesh.position.z + other.boundingBox.minP.z);

            return (selfMax.x >= otherMin.x && selfMax.y >= otherMin.y && selfMax.z >= otherMin.z  &&
                    selfMin.x <= otherMax.x && selfMin.y <= otherMax.y && selfMin.z <= otherMax.z) ||
                   (otherMax.x >= selfMin.x && otherMax.y >= selfMin.y && otherMax.z >= selfMin.z  &&
                    otherMin.x <= selfMax.x && otherMin.y <= selfMax.y && otherMin.z <= selfMax.z);
        }

        if (this.boundingBox.type == "SphereBox" && other.boundingBox.type == "SphereBox") {
            return this.mesh.position.distanceTo(other.mesh.position) <= this.boundingBox.radius + other.boundingBox.radius
        }

        return false;
    }

    this.isOutside = function() {
        if (this.mesh.position.z < 0) return true;
        if (this.boundingBox.type == "SphereBox") {
            return this.mesh.position.x > 100 + this.boundingBox.radius ||
                   this.mesh.position.x < -100 - this.boundingBox.radius;
        }
        if (this.boundingBox.type == "AxisAlignedBox") {
            return this.mesh.position.x > 100 - this.boundingBox.minP.x ||
                   this.mesh.position.x < -100 - this.boundingBox.maxP.x;
        }

        return false;
    }

    // Override me, senpai!
    this.collide = function(other) { 
        //console.error("Collision function not implemented.");
    }
}
