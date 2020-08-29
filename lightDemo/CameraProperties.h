struct OrthoProperties {
	float posX;
	float posY;
	float posZ;

	float lookX;
	float lookY;
	float lookZ;

	float upX;
	float upY;
	float upZ;

	float width;
	float height;

	float nearPlane;
	float farPlane;
};

struct PerspectiveProperties {
	float posX;
	float posY;
	float posZ;

	float lookX;
	float lookY;
	float lookZ;

	float upX;
	float upY;
	float upZ;

	float fov;
	float ratio;
	
	float nearPlane;
	float farPlane;
};

struct PerspectiveProperties perspectiveProperties;
struct PerspectiveProperties carCamProperties;
struct PerspectiveProperties frontCamProperties;
struct PerspectiveProperties backCamProperties;
struct OrthoProperties orthoProperties;
