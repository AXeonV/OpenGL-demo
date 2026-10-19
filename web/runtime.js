var demoAssetBase = new URL('.', document.currentScript.src);
var Module = {
  canvas: document.getElementById('canvas'),
  locateFile: path => new URL(path, demoAssetBase).href,
  print: (...args) => console.log(...args),
  printErr: (...args) => console.error(...args),
};

Module.canvas.addEventListener('pointerdown', () => Module.canvas.focus({ preventScroll: true }));

document.addEventListener('keydown', event => {
  if (event.code !== 'F11') return;
  event.preventDefault();
  if (event.repeat) return;
  const request = document.fullscreenElement
    ? document.exitFullscreen()
    : document.getElementById('canvas-container').requestFullscreen();
  request.catch(error => console.warn('Fullscreen is unavailable in this browser.', error));
}, true);

window.addEventListener('unhandledrejection', event => {
  if (event.reason instanceof DOMException && /pointer.?lock/i.test(event.reason.message)) {
    event.preventDefault();
    console.warn('Pointer lock is unavailable in this browser.', event.reason);
  }
});
