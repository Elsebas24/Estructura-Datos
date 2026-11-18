/*
   Metodos de Ordenamiento simultaneos (version web)

   Cada metodo esta escrito como una "funcion generadora" (function*).
   Cada vez que el metodo compara o mueve un dato hace "yield", es decir,
   se pausa y le avisa al programa que indices esta tocando.
   Asi el programa puede avanzar los 6 metodos un poquito cada cuadro
   y se ven ordenando AL MISMO TIEMPO, igual que los hilos en C.
*/


/* ======================= ALGORITMOS ======================= */

function* burbuja(a, est) {
        const n = a.length;

        for (let i = 0; i < n - 1; i++) {
                for (let j = 0; j < n - 1 - i; j++) {
                        est.comparaciones++;
                        yield { compara: [j, j + 1] };

                        if (a[j] > a[j + 1]) {
                                [a[j], a[j + 1]] = [a[j + 1], a[j]];
                                est.movimientos++;
                                yield { mueve: [j, j + 1] };
                        }
                }
                est.avance = i + 1;
        }
        est.avance = n;
}


function* insercion(a, est) {
        const n = a.length;

        for (let i = 1; i < n; i++) {
                const aux = a[i];
                let j = i - 1;

                while (j >= 0) {
                        est.comparaciones++;
                        yield { compara: [j, i] };

                        if (a[j] <= aux)
                                break;

                        a[j + 1] = a[j];
                        est.movimientos++;
                        yield { mueve: [j, j + 1] };
                        j--;
                }

                a[j + 1] = aux;
                est.avance = i;
        }
        est.avance = n;
}


function* seleccion(a, est) {
        const n = a.length;

        for (let i = 0; i < n - 1; i++) {
                let menor = i;

                for (let j = i + 1; j < n; j++) {
                        est.comparaciones++;
                        yield { compara: [menor, j] };

                        if (a[j] < a[menor])
                                menor = j;
                }

                [a[i], a[menor]] = [a[menor], a[i]];
                est.movimientos++;
                yield { mueve: [i, menor] };
                est.avance = i + 1;
        }
        est.avance = n;
}


function* quicksort(a, est) {
        yield* quickRecursivo(a, 0, a.length - 1, est);
        est.avance = a.length;
}

function* quickRecursivo(a, inicio, fin, est) {
        if (inicio > fin)
                return;

        if (inicio === fin) {
                est.avance++;
                return;
        }

        let i = inicio;
        let j = fin;
        const pivote = a[Math.floor((inicio + fin) / 2)];

        while (i <= j) {
                while (true) {
                        est.comparaciones++;
                        yield { compara: [i, j] };
                        if (a[i] < pivote) i++; else break;
                }
                while (true) {
                        est.comparaciones++;
                        yield { compara: [i, j] };
                        if (a[j] > pivote) j--; else break;
                }

                if (i <= j) {
                        [a[i], a[j]] = [a[j], a[i]];
                        est.movimientos++;
                        yield { mueve: [i, j] };
                        i++;
                        j--;
                }
        }

        if (i - j > 1)
                est.avance += i - j - 1;

        yield* quickRecursivo(a, inicio, j, est);
        yield* quickRecursivo(a, i, fin, est);
}


function* shellsort(a, est) {
        const n = a.length;
        let totalSaltos = 0;
        let paso = 0;

        for (let s = Math.floor(n / 2); s > 0; s = Math.floor(s / 2))
                totalSaltos++;

        for (let salto = Math.floor(n / 2); salto > 0; salto = Math.floor(salto / 2)) {
                for (let i = salto; i < n; i++) {
                        const aux = a[i];
                        let j = i;

                        while (j >= salto) {
                                est.comparaciones++;
                                yield { compara: [j - salto, j] };

                                if (a[j - salto] <= aux)
                                        break;

                                a[j] = a[j - salto];
                                est.movimientos++;
                                yield { mueve: [j - salto, j] };
                                j -= salto;
                        }

                        a[j] = aux;
                        est.avance = Math.floor((paso * n + i) / totalSaltos);
                }
                paso++;
        }
        est.avance = n;
}


function* radix(a, est) {
        const n = a.length;
        const minimo = Math.min(...a);
        const datos = a.map(x => x - minimo);      /* acepta negativos */
        const maximo = Math.max(...datos);

        let pasadas = 0;
        for (let x = maximo; x > 0; x = Math.floor(x / 10))
                pasadas++;

        let exp = 1;

        for (let p = 0; p < pasadas; p++) {
                const cubetas = [[], [], [], [], [], [], [], [], [], []];

                for (let i = 0; i < n; i++) {
                        const digito = Math.floor(datos[i] / exp) % 10;
                        cubetas[digito].push(datos[i]);
                        est.comparaciones++;            /* lectura de digito */
                        yield { compara: [i, i] };
                }

                let k = 0;
                for (let d = 0; d < 10; d++) {
                        for (const valor of cubetas[d]) {
                                datos[k] = valor;
                                a[k] = valor + minimo;
                                est.movimientos++;
                                yield { mueve: [k, k] };
                                k++;
                        }
                }

                est.avance = Math.floor(n * (p + 1) / pasadas);
                exp *= 10;
        }
        est.avance = n;
}


/* ======================= CONFIGURACION ======================= */

const METODOS = [
        { nombre: "Burbuja",   fn: burbuja,   complejidad: "O(n²)" },
        { nombre: "Insercion", fn: insercion, complejidad: "O(n²)" },
        { nombre: "Seleccion", fn: seleccion, complejidad: "O(n²)" },
        { nombre: "Quicksort", fn: quicksort, complejidad: "O(n log n)" },
        { nombre: "ShellSort", fn: shellsort, complejidad: "≈ O(n^1.5)" },
        { nombre: "Radix",     fn: radix,     complejidad: "O(d·n)" }
];

const COLOR = {
        dato:    "#4b5568",
        compara: "#f5b942",
        mueve:   "#ff5d73",
        listo:   "#3ddc97"
};

let original = [];
let paneles = [];
let corriendo = false;
let lugar = 0;

const $ = id => document.getElementById(id);


/* ======================= DATOS ======================= */

function generarDatos() {
        const n = Number($("cantidad").value);
        const tipo = $("tipoDatos").value;

        original = [];
        for (let i = 0; i < n; i++)
                original.push(5 + Math.floor(Math.random() * 96));     /* 5..100 */

        if (tipo === "casi" || tipo === "inverso")
                original.sort((x, y) => x - y);

        if (tipo === "inverso")
                original.reverse();

        if (tipo === "casi") {
                for (let k = 0; k < Math.max(2, n / 10); k++) {
                        const i = Math.floor(Math.random() * n);
                        const j = Math.floor(Math.random() * n);
                        [original[i], original[j]] = [original[j], original[i]];
                }
        }

        crearPaneles();
        limpiarTabla();
}


/* ======================= PANELES ======================= */

function crearPaneles() {
        const tablero = $("tablero");
        tablero.innerHTML = "";
        paneles = [];

        for (const m of METODOS) {
                const div = document.createElement("div");
                div.className = "metodo";
                div.innerHTML = `
                        <div class="lugar"></div>
                        <div class="metodo-cabeza">
                                <span class="metodo-nombre">${m.nombre}</span>
                                <span class="metodo-complejidad">${m.complejidad}</span>
                        </div>
                        <canvas></canvas>
                        <div class="progreso">
                                <div class="barra"><div class="relleno"></div></div>
                                <span class="porcentaje">0%</span>
                        </div>
                        <div class="estadisticas">
                                <span>Comparaciones <b class="comp">0</b></span>
                                <span>Movimientos <b class="mov">0</b></span>
                        </div>`;
                tablero.appendChild(div);

                const panel = {
                        metodo: m,
                        div: div,
                        canvas: div.querySelector("canvas"),
                        datos: original.slice(),         /* copia propia */
                        est: { comparaciones: 0, movimientos: 0, avance: 0 },
                        marca: null,
                        gen: null,
                        terminado: false,
                        lugar: 0
                };

                paneles.push(panel);
                dibujar(panel);
        }
}

function dibujar(p) {
        const c = p.canvas;
        const escala = window.devicePixelRatio || 1;
        c.width = c.clientWidth * escala;
        c.height = c.clientHeight * escala;

        const ctx = c.getContext("2d");
        const n = p.datos.length;
        const ancho = c.width / n;
        const separacion = n > 100 ? 0 : 1 * escala;

        ctx.clearRect(0, 0, c.width, c.height);

        for (let i = 0; i < n; i++) {
                let color = COLOR.dato;

                if (p.terminado)
                        color = COLOR.listo;
                else if (p.marca && p.marca.mueve && p.marca.mueve.includes(i))
                        color = COLOR.mueve;
                else if (p.marca && p.marca.compara && p.marca.compara.includes(i))
                        color = COLOR.compara;

                const alto = (p.datos[i] / 100) * (c.height - 6 * escala);
                ctx.fillStyle = color;
                ctx.fillRect(i * ancho, c.height - alto, ancho - separacion, alto);
        }

        const n2 = p.datos.length;
        const porcentaje = Math.floor(p.est.avance * 100 / n2);
        p.div.querySelector(".relleno").style.width = porcentaje + "%";
        p.div.querySelector(".porcentaje").textContent = porcentaje + "%";
        p.div.querySelector(".comp").textContent = p.est.comparaciones.toLocaleString();
        p.div.querySelector(".mov").textContent = p.est.movimientos.toLocaleString();
}


/* ======================= CARRERA ======================= */

function ordenar() {
        if (corriendo)
                return;

        crearPaneles();     /* reinicia con los mismos datos originales */
        limpiarTabla();

        for (const p of paneles)
                p.gen = p.metodo.fn(p.datos, p.est);

        lugar = 0;
        corriendo = true;
        $("btnOrdenar").disabled = true;
        $("btnGenerar").disabled = true;

        requestAnimationFrame(cuadro);
}

/* Se ejecuta ~60 veces por segundo: avanza a todos los metodos */
function cuadro() {
        const pasos = Number($("velocidad").value);
        let activos = 0;

        for (const p of paneles) {
                if (p.terminado)
                        continue;

                activos++;

                for (let k = 0; k < pasos; k++) {
                        const r = p.gen.next();

                        if (r.done) {
                                p.terminado = true;
                                p.marca = null;
                                p.lugar = ++lugar;
                                p.div.classList.add("terminado");
                                p.div.querySelector(".lugar").textContent = "#" + p.lugar;
                                agregarResultado(p);
                                break;
                        }

                        p.marca = r.value;
                }

                dibujar(p);
        }

        if (activos > 0) {
                requestAnimationFrame(cuadro);
        } else {
                corriendo = false;
                $("btnOrdenar").disabled = false;
                $("btnGenerar").disabled = false;
        }
}


/* ======================= TABLA ======================= */

function limpiarTabla() {
        $("tablaResultados").innerHTML =
                `<tr><td colspan="6" class="vacio">Presiona “Ordenar” para iniciar la carrera.</td></tr>`;
}

function agregarResultado(p) {
        const tabla = $("tablaResultados");

        if (p.lugar === 1)
                tabla.innerHTML = "";

        const ops = p.est.comparaciones + p.est.movimientos;
        const fila = document.createElement("tr");

        if (p.lugar === 1)
                fila.className = "primero";

        fila.innerHTML = `
                <td>#${p.lugar}</td>
                <td>${p.metodo.nombre}</td>
                <td>${p.metodo.complejidad}</td>
                <td>${p.est.comparaciones.toLocaleString()}</td>
                <td>${p.est.movimientos.toLocaleString()}</td>
                <td>${ops.toLocaleString()}</td>`;

        tabla.appendChild(fila);
}


/* ======================= EVENTOS ======================= */

$("cantidad").addEventListener("input", e => {
        $("cantidadTexto").textContent = e.target.value;
        if (!corriendo) generarDatos();
});

$("velocidad").addEventListener("input", e => {
        $("velocidadTexto").textContent = e.target.value;
});

$("tipoDatos").addEventListener("change", () => {
        if (!corriendo) generarDatos();
});

$("btnGenerar").addEventListener("click", generarDatos);
$("btnOrdenar").addEventListener("click", ordenar);

window.addEventListener("resize", () => paneles.forEach(dibujar));

generarDatos();
