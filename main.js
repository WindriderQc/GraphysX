import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

// Scene setup
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x87ceeb); // Sky blue
scene.fog = new THREE.Fog(0x87ceeb, 10, 50);

// Camera setup
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(10, 10, 20);

// Renderer setup
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.setPixelRatio(window.devicePixelRatio);
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;
document.getElementById('app').appendChild(renderer.domElement);

// Controls
const controls = new OrbitControls(camera, renderer.domElement);
controls.enableDamping = true;
controls.dampingFactor = 0.05;

// Lighting
const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
scene.add(ambientLight);

const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
directionalLight.position.set(5, 10, 7.5);
directionalLight.castShadow = true;
// Set up shadow properties for the light
directionalLight.shadow.mapSize.width = 1024;
directionalLight.shadow.mapSize.height = 1024;
directionalLight.shadow.camera.left = -10;
directionalLight.shadow.camera.right = 10;
directionalLight.shadow.camera.top = 10;
directionalLight.shadow.camera.bottom = -10;
scene.add(directionalLight);

// Planche (Large Plane)
const planeGeometry = new THREE.PlaneGeometry(100, 100);
const planeMaterial = new THREE.MeshStandardMaterial({
  color: 0x348C31, // Grass green
  roughness: 0.8,
  metalness: 0.2
});
const plane = new THREE.Mesh(planeGeometry, planeMaterial);
plane.rotation.x = -Math.PI / 2;
plane.receiveShadow = true;
scene.add(plane);

// Obstacles
function createObstacle(x, z, type) {
  let geometry;
  if (type === 'box') {
    geometry = new THREE.BoxGeometry(2, Math.random() * 5 + 1, 2);
  } else {
    geometry = new THREE.CylinderGeometry(1, 1, Math.random() * 5 + 1, 16);
  }
  const material = new THREE.MeshStandardMaterial({ color: Math.random() * 0xffffff });
  const mesh = new THREE.Mesh(geometry, material);
  mesh.position.set(x, geometry.parameters.height / 2, z);
  mesh.castShadow = true;
  mesh.receiveShadow = true;
  scene.add(mesh);
}

// Add some random obstacles
for (let i = 0; i < 20; i++) {
  const x = (Math.random() - 0.5) * 40;
  const z = (Math.random() - 0.5) * 40;
  if (Math.abs(x) > 5 || Math.abs(z) > 5) { // Don't place on the starting area
    createObstacle(x, z, Math.random() > 0.5 ? 'box' : 'cylinder');
  }
}

// Ball (Sphere)
const ballGeometry = new THREE.SphereGeometry(1, 32, 32);
const ballMaterial = new THREE.MeshStandardMaterial({ color: 0xff0000 });
const ball = new THREE.Mesh(ballGeometry, ballMaterial);
ball.position.y = 1; // Radius is 1, so position at 1 to sit on the plane
ball.castShadow = true;
scene.add(ball);

// Keyboard controls state
const keys = {
  w: false,
  a: false,
  s: false,
  d: false,
};

window.addEventListener('keydown', (e) => {
  if (keys.hasOwnProperty(e.key.toLowerCase())) {
    keys[e.key.toLowerCase()] = true;
  }
});

window.addEventListener('keyup', (e) => {
  if (keys.hasOwnProperty(e.key.toLowerCase())) {
    keys[e.key.toLowerCase()] = false;
  }
});

const ballVelocity = new THREE.Vector3();
const acceleration = 0.05;
const friction = 0.95;

// Window resize handler
window.addEventListener('resize', () => {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
});

// Animation loop
function animate() {
  requestAnimationFrame(animate);

  // Update ball position based on keys
  if (keys.w) ballVelocity.z -= acceleration;
  if (keys.s) ballVelocity.z += acceleration;
  if (keys.a) ballVelocity.x -= acceleration;
  if (keys.d) ballVelocity.x += acceleration;

  // Apply friction
  ballVelocity.multiplyScalar(friction);

  // Update position
  ball.position.x += ballVelocity.x;
  ball.position.z += ballVelocity.z;

  // Simple bounce animation on top of movement
  ball.position.y = 1 + Math.abs(Math.sin(Date.now() * 0.002) * 2);

  // Make camera follow ball (optional, but good for "controls")
  // camera.position.x += ballVelocity.x;
  // camera.position.z += ballVelocity.z;
  // controls.target.copy(ball.position);

  controls.update();
  renderer.render(scene, camera);
}

animate();
