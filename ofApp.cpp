#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(300, 8);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();

	auto radius = 200;
	auto noise_seed = ofRandom(1000);
	for (int i = 0; i < this->triangle_list.size(); i++) {

		glm::vec3 avg = (this->triangle_list[i].getVertex(0) + this->triangle_list[i].getVertex(1) + this->triangle_list[i].getVertex(2)) / 3;
		auto noise_value = ofNoise(noise_seed + ofGetFrameNum() * 0.01, avg.y * 0.005, avg.x * 0.005, avg.z * 0.005);

		if (noise_value < 0.47 || noise_value > 0.53) { continue; }

		vector<glm::vec3> vertices;

		vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(0)) * radius);
		vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(1)) * radius);
		vertices.push_back(glm::normalize(this->triangle_list[i].getVertex(2)) * radius);

		this->mesh.addVertices(vertices);
		this->frame.addVertices(vertices);

		ofColor color;
		color.setHsb(ofMap(ofNoise(glm::vec4(noise_seed, avg * 0.0005)), 0, 1, 0, 255), 255, 255);

		for (int k = 0; k < vertices.size(); k++) {

			this->mesh.addColor(color);
			this->frame.addColor(color);
		}

		this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);

		this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
		this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
		this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 1);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());

	this->mesh.draw();
	this->frame.drawWireframe();

	this->cam.end();

	ostringstream os;
	os << setw(4) << setfill('0') << ofGetFrameNum();
	ofImage image;
	image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	image.saveImage("image/img_" + os.str() + ".jpg");
	if (ofGetFrameNum() >= 30 * 20 + 1) {

		std::exit(1);
	}

	// > ffmpeg -i img_%04d.jpg -r 24 out.mp4
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}