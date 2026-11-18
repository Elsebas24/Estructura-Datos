/* ============================================================
   CAMINOS DEL GRAFO — versión web del programa en C
   ============================================================
   1) Se arma la lista de adyacencia con los vértices declarados.
   2) Se buscan todos los caminos con DFS + vuelta atrás.
   3) Cada camino se pinta en el dibujo con su propio color
      y se agrega una fila a la tabla.
   ============================================================ */


// ---------- Datos del grafo (los mismos del main en C) ----------

const etiquetas = [1, 2, 3, 4, 5, 6, 8, 9, 10];

const aristas = [
    [1, 3], [1, 5], [2, 3], [2, 5], [3, 4],
    [4, 6], [4, 10], [5, 6], [5, 9], [6, 8],
    [6, 9], [6, 10], [8, 9]
];


// ---------- Lista de adyacencia ----------

// adyacencia[v] guarda los vecinos del vértice v
const adyacencia = {};
const aristasValidas = [];

function crearGrafo() {
    for (const v of etiquetas) {
        adyacencia[v] = [];
    }

    for (const [a, b] of aristas) {

        // Solo se aceptan vértices declarados
        if (!(a in adyacencia) || !(b in adyacencia)) {
            console.warn(`Arista (${a}, ${b}): vértice no declarado`);
            continue;
        }

        // No se permiten aristas repetidas
        if (adyacencia[a].includes(b)) {
            continue;
        }

        // Se inserta al inicio, igual que en C,
        // así los caminos salen en el mismo orden
        adyacencia[a].unshift(b);
        if (a !== b) {
            adyacencia[b].unshift(a);
        }
        aristasValidas.push([a, b]);
    }
}


// ---------- Búsqueda de caminos (DFS + vuelta atrás) ----------

function buscarCaminos(inicio, fin) {
    const caminos = [];
    const camino = [];
    const visitado = {};

    function dfs(actual) {
        visitado[actual] = true;
        camino.push(actual);

        if (actual === fin) {
            caminos.push([...camino]);     // se guarda una copia
        } else {
            for (const vecino of adyacencia[actual]) {
                if (!visitado[vecino]) {
                    dfs(vecino);
                }
            }
        }

        // Vuelta atrás: se libera el vértice para otros caminos
        camino.pop();
        visitado[actual] = false;
    }

    dfs(inicio);
    return caminos;
}


// ---------- Posición de cada vértice en el dibujo ----------

const ANCHO = 640;
const ALTO = 420;
const MARGEN = 48;
const RADIO = 19;
const posiciones = {};

// Acomodo por "fuerzas": los vértices se empujan entre sí
// y cada arista jala a sus dos extremos como un resorte
function calcularPosiciones() {
    const n = etiquetas.length;
    const pos = {};

    // Se empieza con los vértices en círculo
    etiquetas.forEach((v, i) => {
        const angulo = (2 * Math.PI * i) / n;
        pos[v] = { x: Math.cos(angulo), y: Math.sin(angulo) };
    });

    const ideal = 1.6 / Math.sqrt(n);   // distancia ideal
    let paso = 0.1;

    for (let vuelta = 0; vuelta < 400; vuelta++) {
        const mov = {};
        for (const v of etiquetas) {
            mov[v] = { x: 0, y: 0 };
        }

        // Repulsión entre todos los vértices
        for (const a of etiquetas) {
            for (const b of etiquetas) {
                if (a === b) {
                    continue;
                }
                const dx = pos[a].x - pos[b].x;
                const dy = pos[a].y - pos[b].y;
                const d = Math.hypot(dx, dy) || 0.01;
                const fuerza = (ideal * ideal) / d;
                mov[a].x += (dx / d) * fuerza;
                mov[a].y += (dy / d) * fuerza;
            }
        }

        // Atracción de las aristas
        for (const [a, b] of aristasValidas) {
            const dx = pos[a].x - pos[b].x;
            const dy = pos[a].y - pos[b].y;
            const d = Math.hypot(dx, dy) || 0.01;
            const fuerza = (d * d) / ideal;
            mov[a].x -= (dx / d) * fuerza;
            mov[a].y -= (dy / d) * fuerza;
            mov[b].x += (dx / d) * fuerza;
            mov[b].y += (dy / d) * fuerza;
        }

        // Cada vértice se mueve un paso pequeño
        for (const v of etiquetas) {
            const d = Math.hypot(mov[v].x, mov[v].y) || 0.01;
            const avance = Math.min(d, paso);
            pos[v].x += (mov[v].x / d) * avance;
            pos[v].y += (mov[v].y / d) * avance;
        }
        paso *= 0.99;
    }

    // Si quedó más alto que ancho, se gira 90° (se cambian x, y)
    const altoIni = Math.max(...etiquetas.map(v => pos[v].y)) -
                    Math.min(...etiquetas.map(v => pos[v].y));
    const anchoIni = Math.max(...etiquetas.map(v => pos[v].x)) -
                     Math.min(...etiquetas.map(v => pos[v].x));
    if (altoIni > anchoIni) {
        for (const v of etiquetas) {
            pos[v] = { x: pos[v].y, y: pos[v].x };
        }
    }

    // Se escala todo al tamaño del dibujo
    const xs = etiquetas.map(v => pos[v].x);
    const ys = etiquetas.map(v => pos[v].y);
    const minX = Math.min(...xs);
    const minY = Math.min(...ys);
    const anchoG = Math.max(...xs) - minX || 1;
    const altoG = Math.max(...ys) - minY || 1;

    const anchoUtil = ANCHO - 2 * MARGEN;
    const altoUtil = ALTO - 2 * MARGEN;
    const escala = Math.min(anchoUtil / anchoG, altoUtil / altoG);
    const sobraX = (anchoUtil - anchoG * escala) / 2;
    const sobraY = (altoUtil - altoG * escala) / 2;

    for (const v of etiquetas) {
        posiciones[v] = {
            x: MARGEN + sobraX + (pos[v].x - minX) * escala,
            y: MARGEN + sobraY + (pos[v].y - minY) * escala
        };
    }
}


// ---------- Dibujo base (aristas y vértices) ----------

const SVG = "http://www.w3.org/2000/svg";

function crearSvg(etiqueta, atributos, padre) {
    const elemento = document.createElementNS(SVG, etiqueta);
    for (const nombre in atributos) {
        elemento.setAttribute(nombre, atributos[nombre]);
    }
    padre.appendChild(elemento);
    return elemento;
}

let capaCaminos;

function dibujarGrafo() {
    const svg = document.getElementById("dibujo");
    svg.setAttribute("viewBox", `0 0 ${ANCHO} ${ALTO}`);

    // Tres capas: aristas abajo, caminos en medio, vértices arriba
    const capaAristas = crearSvg("g", {}, svg);
    capaCaminos = crearSvg("g", {}, svg);
    const capaVertices = crearSvg("g", {}, svg);

    for (const [a, b] of aristasValidas) {
        crearSvg("line", {
            class: "arista-base",
            x1: posiciones[a].x,
            y1: posiciones[a].y,
            x2: posiciones[b].x,
            y2: posiciones[b].y
        }, capaAristas);
    }

    for (const v of etiquetas) {
        const { x, y } = posiciones[v];
        const grupo = crearSvg("g", {
            class: "vertice",
            id: "vertice-" + v
        }, capaVertices);

        crearSvg("circle", { cx: x, cy: y, r: RADIO }, grupo);

        const numero = crearSvg("text", {
            class: "numero", x: x, y: y
        }, grupo);
        numero.textContent = v;

        crearSvg("text", {
            class: "rol", x: x, y: y - RADIO - 9
        }, grupo);
    }
}

// Resalta el vértice de inicio y el final
function marcarExtremos(inicio, fin) {
    for (const v of etiquetas) {
        const grupo = document.getElementById("vertice-" + v);
        grupo.classList.remove("inicio", "fin");
        grupo.querySelector(".rol").textContent = "";
    }

    const gInicio = document.getElementById("vertice-" + inicio);
    const gFin = document.getElementById("vertice-" + fin);

    gInicio.classList.add("inicio");
    gFin.classList.add("fin");

    if (inicio === fin) {
        gInicio.querySelector(".rol").textContent = "INICIO · FIN";
    } else {
        gInicio.querySelector(".rol").textContent = "INICIO";
        gFin.querySelector(".rol").textContent = "FIN";
    }
}


// ---------- Pintar los caminos ----------

// Color de cada camino: se gira el tono 137.5° cada vez
// para que caminos seguidos tengan colores muy distintos
function tono(i) {
    return Math.round((i * 137.508) % 360);
}

function claveArista(a, b) {
    return Math.min(a, b) + "-" + Math.max(a, b);
}

// Para cada arista, qué caminos pasan por ella
function contarUso(caminos) {
    const uso = {};
    caminos.forEach((c, i) => {
        for (let j = 0; j < c.length - 1; j++) {
            const clave = claveArista(c[j], c[j + 1]);
            if (!uso[clave]) {
                uso[clave] = [];
            }
            uso[clave].push(i);
        }
    });
    return uso;
}

// Puntos de la línea de un camino. Si varios caminos usan
// la misma arista, cada uno se recorre un poco hacia un lado
// para que se vean todos juntos sin taparse
function puntosCamino(c, i, uso) {
    const puntos = [];

    for (let j = 0; j < c.length - 1; j++) {
        const a = c[j];
        const b = c[j + 1];

        const lista = uso[claveArista(a, b)];
        const lugar = lista.indexOf(i);
        const separacion = Math.min(4, 24 / lista.length);
        const desp = (lugar - (lista.length - 1) / 2) * separacion;

        // Dirección perpendicular a la arista
        const p1 = posiciones[Math.min(a, b)];
        const p2 = posiciones[Math.max(a, b)];
        const dx = p2.x - p1.x;
        const dy = p2.y - p1.y;
        const d = Math.hypot(dx, dy);
        const nx = (-dy / d) * desp;
        const ny = (dx / d) * desp;

        const pa = posiciones[a];
        const pb = posiciones[b];
        puntos.push(`${pa.x + nx},${pa.y + ny}`);
        puntos.push(`${pb.x + nx},${pb.y + ny}`);
    }

    return puntos.join(" ");
}

function agregarCamino(c, i, uso, animar) {
    if (c.length < 2) {
        return;     // inicio = fin, no hay línea que pintar
    }
    const linea = crearSvg("polyline", {
        class: animar ? "camino dibujando" : "camino",
        points: puntosCamino(c, i, uso),
        pathLength: 1,
        "data-i": i
    }, capaCaminos);
    linea.style.setProperty("--h", tono(i));
}


// ---------- Tabla y resumen ----------

let cuerpoTabla;
let enfocado = -1;      // -1 = se ven todos los caminos

function agregarFila(c, i, minimo, animar) {
    const largo = c.length - 1;
    const fila = document.createElement("tr");

    fila.dataset.i = i;
    fila.tabIndex = 0;
    fila.style.setProperty("--h", tono(i));
    if (animar) {
        fila.classList.add("fila-nueva");
    }

    const flecha = '<span class="flecha"> → </span>';
    const insignia = largo === minimo
        ? '<span class="insignia">MÁS CORTO</span>'
        : "";

    fila.innerHTML = `
        <td class="num">${i + 1}</td>
        <td><span class="muestra"></span></td>
        <td class="recorrido">${c.join(flecha)}</td>
        <td class="num">${largo}</td>
        <td>${insignia}</td>`;

    fila.addEventListener("click", () => enfocar(i));
    fila.addEventListener("keydown", e => {
        if (e.key === "Enter" || e.key === " ") {
            e.preventDefault();
            enfocar(i);
        }
    });

    cuerpoTabla.appendChild(fila);
}

function mostrarResumen(caminos) {
    const total = document.getElementById("total");
    const minimo = document.getElementById("minimo");
    const maximo = document.getElementById("maximo");

    total.textContent = caminos.length;

    if (caminos.length === 0) {
        minimo.textContent = "—";
        maximo.textContent = "—";
        return;
    }

    const largos = caminos.map(c => c.length - 1);
    const menor = Math.min(...largos);
    const mayor = Math.max(...largos);
    const cuantos = largos.filter(l => l === menor).length;
    const texto = cuantos === 1 ? "camino" : "caminos";

    minimo.innerHTML =
        `${menor}<small> aristas · ${cuantos} ${texto}</small>`;
    maximo.innerHTML = `${mayor}<small> aristas</small>`;
}

function enfocar(i) {
    enfocado = (enfocado === i) ? -1 : i;
    aplicarEnfoque();
}

function aplicarEnfoque() {
    capaCaminos.classList.toggle("con-enfoque", enfocado !== -1);

    for (const linea of [...capaCaminos.children]) {
        const activo = Number(linea.dataset.i) === enfocado;
        linea.classList.toggle("activo", activo);
        if (activo) {
            capaCaminos.appendChild(linea);     // se pasa al frente
        }
    }

    for (const fila of cuerpoTabla.children) {
        const activo = Number(fila.dataset.i) === enfocado;
        fila.classList.toggle("activo", activo);
    }

    const ayuda = document.getElementById("ayuda");
    if (enfocado === -1) {
        ayuda.textContent =
            "Toca un camino de la tabla para verlo solo.";
    } else {
        ayuda.textContent =
            `Viendo el camino ${enfocado + 1}. ` +
            "Tócalo otra vez para ver todos.";
    }
}


// ---------- Proceso completo ----------

let turno = 0;      // sirve para cancelar una animación anterior

function esperar(ms) {
    return new Promise(listo => setTimeout(listo, ms));
}

async function pintar(animar) {
    const miTurno = ++turno;

    const inicio = Number(document.getElementById("inicio").value);
    const fin = Number(document.getElementById("fin").value);
    const caminos = buscarCaminos(inicio, fin);

    // Se limpia lo anterior
    capaCaminos.replaceChildren();
    cuerpoTabla.replaceChildren();
    enfocado = -1;
    aplicarEnfoque();

    marcarExtremos(inicio, fin);
    mostrarResumen(caminos);

    if (caminos.length === 0) {
        cuerpoTabla.innerHTML = `
            <tr class="vacio"><td colspan="5">
                No existe ningún camino de ${inicio} a ${fin}.
            </td></tr>`;
        return;
    }

    const reducir =
        matchMedia("(prefers-reduced-motion: reduce)").matches;
    if (reducir) {
        animar = false;
    }

    const largos = caminos.map(c => c.length - 1);
    const minimo = Math.min(...largos);
    const uso = contarUso(caminos);

    // Se van pintando uno por uno
    for (let i = 0; i < caminos.length; i++) {
        if (miTurno !== turno) {
            return;     // se pidió otra búsqueda
        }
        agregarCamino(caminos[i], i, uso, animar);
        agregarFila(caminos[i], i, minimo, animar);
        if (animar) {
            await esperar(380);
        }
    }
}

function llenarSelect(select, valor) {
    for (const v of etiquetas) {
        const opcion = document.createElement("option");
        opcion.value = v;
        opcion.textContent = v;
        select.appendChild(opcion);
    }
    select.value = valor;
}

function iniciar() {
    crearGrafo();
    calcularPosiciones();
    dibujarGrafo();

    cuerpoTabla = document.getElementById("cuerpo-tabla");

    llenarSelect(document.getElementById("inicio"), etiquetas[0]);
    llenarSelect(document.getElementById("fin"),
                 etiquetas[etiquetas.length - 1]);

    document.getElementById("nota-grafo").textContent =
        `${etiquetas.length} vértices · ` +
        `${aristasValidas.length} aristas`;

    document.getElementById("controles")
        .addEventListener("submit", e => {
            e.preventDefault();
            pintar(true);
        });

    // Primera vista: de 1 a 10, ya pintado
    pintar(false);
}

document.addEventListener("DOMContentLoaded", iniciar);