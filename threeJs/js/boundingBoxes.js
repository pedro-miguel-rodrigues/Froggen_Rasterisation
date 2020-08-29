function AxisAlignedBox(p1, p2) {
    this.maxP = p1;
    this.minP = p2;
    this.type = "AxisAlignedBox";
};

function SphereBox(r) {
    this.radius = r;
    this.type = "SphereBox";
};