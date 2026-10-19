import { spawnSync } from 'node:child_process';
import { existsSync, mkdirSync, readFileSync, readdirSync, writeFileSync } from 'node:fs';
import { delimiter, dirname, join, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../', import.meta.url));
const option = name => {
  const index = process.argv.indexOf(name);
  if (index < 0) return null;
  if (!process.argv[index + 1] || process.argv[index + 1].startsWith('--')) throw new Error(`Missing value for ${name}`);
  return process.argv[index + 1];
};
const output = resolve(option('--out') || root);
const sdk = option('--emsdk') || process.env.EMSDK;
const cache = join(root, '.web-cache');
const source = join(cache, 'src');
const shaders = join(cache, 'res/shaders');
const utf8 = new TextDecoder('utf-8', { fatal: true });
const legacy = new TextDecoder('gb18030', { fatal: true });
const decode = path => {
  const bytes = readFileSync(path);
  let text;
  try { text = utf8.decode(bytes); } catch { text = legacy.decode(bytes); }
  return text.replaceAll('\r\n', '\n');
};
function save(path, contents) {
  mkdirSync(dirname(path), { recursive: true });
  writeFileSync(path, contents);
}

// Work only on generated copies; keep the original desktop source encoding and GLSL intact.
for (const file of ['Application.cpp', 'Camera.h', 'Shader.h', 'GLPlatform.h']) {
  save(join(source, file), decode(join(root, 'OpenGL/src', file)));
}
for (const program of ['cube', 'light']) {
  for (const stage of ['vertex', 'fragment']) {
    let code = decode(join(root, 'OpenGL/res/shaders', program, `${stage}.glsl`));
    code = code.replace('#version 330 core', '#version 300 es\nprecision highp float;\nprecision highp int;');
    code = code.replace(/\b(\d+(?:\.\d*)?(?:e[+-]?\d+)?)f\b/gi, '$1');
    if (stage === 'fragment') {
      code = code.replace('precision highp int;', 'precision highp int;\nout vec4 webFragColor;').replaceAll('gl_FragColor', 'webFragColor');
      // The desktop shader leaves this numerically exceptional path undefined.
      // Keep all three existing face tests; supply a return for strict ES compilers.
      if (program === 'cube') code = code.replace(
        'if (1.0 - abs(modelPos.z) < EPS) return vec2(modelPos.x, modelPos.y);',
        'if (1.0 - abs(modelPos.z) < EPS) return vec2(modelPos.x, modelPos.y);\n\treturn vec2(modelPos.x, modelPos.y);',
      );
    }
    save(join(shaders, program, `${stage}.glsl`), code);
  }
}

for (const file of ['index.html', 'web/runtime.js', 'web/style.css']) {
  if (resolve(root, file) !== resolve(output, file)) save(join(output, file), readFileSync(join(root, file)));
}
mkdirSync(join(output, 'web'), { recursive: true });
const args = [
  join(source, 'Application.cpp'), '-std=c++17', '-O2', '-fexceptions', '-DGLFW_INCLUDE_NONE',
  '-I', join(root, 'Dependencies'), '--use-port=contrib.glfw3',
  '-sMIN_WEBGL_VERSION=2', '-sMAX_WEBGL_VERSION=2', '-sALLOW_MEMORY_GROWTH=1',
  '-sDISABLE_EXCEPTION_CATCHING=0',
  '--preload-file', `${join(cache, 'res')}@/res`,
  '-o', 'web/app.js',
];
let command = 'em++';
const env = { ...process.env };
if (sdk) {
  const directory = resolve(sdk);
  const driver = join(directory, 'upstream/emscripten/em++.py');
  if (!existsSync(driver)) throw new Error(`No Emscripten compiler in ${directory}. Install and activate SDK 6.0.12 first.`);
  env.EMSDK = directory;
  env.EM_CONFIG = join(directory, '.emscripten');
  command = process.platform === 'win32'
    ? join(directory, 'python', readdirSync(join(directory, 'python')).find(name => existsSync(join(directory, 'python', name, 'python.exe'))) || '', 'python.exe')
    : 'python3';
  if (process.platform === 'win32') {
    env.EMSDK_PYTHON = command;
    env.PATH = [dirname(command), join(directory, 'upstream/emscripten'), env.PATH].join(delimiter);
  }
  args.unshift(driver);
}
console.log('Compiling the original C++ scene for WebGL 2…');
const result = spawnSync(command, args, { cwd: output, env, stdio: 'inherit' });
if (result.error) throw new Error('Unable to run Emscripten. Activate emsdk_env first or pass --emsdk <SDK directory>.', { cause: result.error });
if (result.status !== 0) process.exit(result.status || 1);
for (const file of ['app.js', 'app.wasm', 'app.data']) {
  if (!existsSync(join(output, 'web', file))) throw new Error(`Missing Web build output: ${file}`);
}
console.log(`Web build ready: ${join(output, 'index.html')}`);
