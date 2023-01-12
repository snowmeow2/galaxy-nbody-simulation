import * as THREE from 'three';
import { OrbitControls } from 'https://unpkg.com/three@0.122.0/examples/jsm/controls/OrbitControls.js';

const camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 3000);
camera.position.z = 500;

const light = new THREE.DirectionalLight(0xffffff);
light.position.set(1, -1, 1).normalize();

const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);

new OrbitControls(camera, renderer.domElement);

let clock = new THREE.Clock(true);
let scene = new THREE.Scene();


const particles_pos_file_prefix = "out/halo_";
const frames = 50;


async function loadPosition(prefix, frames) {
    let particles_pos_history = [];

    // loop the files
    for (let i = 0; i < frames; i++) {
        let file_name = prefix + i * 100 + ".txt";
        let particles_pos = [];

        // load the file
        await fetch(file_name).then(res => res.text()).then(data => {
            let lines = data.split('\n');
            for (let j = 0; j < lines.length; j++) {
                let line = lines[j];
                let parts = line.split(' ');
                if ((parts.length != 6) && (parts.length != 3)) {
                    continue;
                }
                particles_pos.push([parseFloat(parts[0]), parseFloat(parts[1]), parseFloat(parts[2])]);
            }
            particles_pos_history.push(particles_pos);
        });
    }
    return particles_pos_history;
}

async function init() {
    const particle_geometry = new THREE.Geometry();
    const particles_pos_history = await loadPosition(particles_pos_file_prefix, frames);
    const total_particles = particles_pos_history[0].length;

    // create the vertices
    for (let i = 0; i < total_particles; i++) {
        let pos = particles_pos_history[0][i];
        particle_geometry.vertices.push(new THREE.Vector3(pos[0], pos[1], pos[2]));
    }

    let particle_material = new THREE.PointsMaterial({
        color: 0xffffff,
        size: .2,
        // load the texture
        // map: THREE.ImageUtils.loadTexture("disc.png"),
        blending: THREE.AdditiveBlending,
        emissive: 0xffffff,
        transparent: true,
    });
    let particle_system = new THREE.Points(particle_geometry, particle_material);

    scene.add(light);
    scene.add(particle_system);

    document.body.appendChild(renderer.domElement);
    window.addEventListener('resize', onWindowResize, false);
    render();
    return [particles_pos_history, particle_system];
}

var start = performance.now()
init().then((data) => {
    let particles_pos_history = data[0];
    let particle_system = data[1];

    let count = 0;
    let total_frames = particles_pos_history.length;

    console.log(total_frames);
    function animate() {
        var verts = particle_system.geometry.vertices;

        // deltaTime = clock.getElapsedTime();
        // console.log(deltaTime);

        if ((count + 10) / 10 >= total_frames) {
            count = 0;
        }

        // if (count/10 % 1 == 0) {
        for (var i = 0; i < verts.length; i++) {
            var vert = verts[i];
            // round count to the nearest 1
            let pos = particles_pos_history[Math.round(count / 10)][i];
            let pos_v = new THREE.Vector3(pos[0], pos[1], pos[2]);
            vert.lerp(pos_v, 0.1);
        }
        // }
        particle_system.geometry.verticesNeedUpdate = true;

        count++;
        render();
        requestAnimationFrame(animate);
    }

    animate();
});

function render() {
    renderer.render(scene, camera);
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
    render();
}
