#Experiment
	
### Параметры эксперимента

#### Параметры сеток поля и частиц

**Значения:**
* $L = 1$ - длина расчётной области по оси $x$.

**Разрешение сетки:** $$n_x = 64, \ \ \ n_y=8.$$ 
**Пространственный шаг:**
$$\Delta x = \dfrac{L}{64}, \ \ \ \Delta y = \dfrac{L}{64}.$$


#### Параметры распределения частиц

**Значения:**
- $k = 0.5$ - число ячеек на дебаевскую длину;
- $l = \Delta x \cdot k$ - дебаевская длина;
- $avg = 30$ - среднее число частиц на ячейку;
- $L = 1$ - длина расчётной области по оси $x$;

- $t = 10^{-2} \cdot m_e \cdot c^2$ - начальная температура частиц;

- $density = \dfrac{t}{8 \pi (e^- \cdot l )^2}$ - начальная плотность частиц.  

**Тип частицы:** электрон.

**Функция плотности распределения:**
$$ d(r) = density \cdot (1 + 0.05 \cdot sin( \dfrac{2\pi \cdot r_x}{L})).$$
**Функция температуры:**
$$ t(r) = temp. $$
**Функция начального импульса:**
$$ p(r) = 0. $$
**Функция веса частиц:**
$$ w(r) = \dfrac{density \cdot \Delta x \cdot \Delta y \cdot \Delta z}{avg}. $$


#### Параметры поля

**Значения:**
- $L = 1$ - длина расчётной области по оси $x$;
- $density = \dfrac{t}{8 \pi (e^- \cdot l )^2}$ - начальная плотность частиц;  
- $A = 2 \cdot L \cdot density \cdot e^- \cdot 0.05$ - амплитуда колебаний.

**Функция начального электрического поля:**
$$ E_0(r) = \left( -A \cdot cos(\dfrac{2 \pi \cdot r_x}{L}), \ 0, \ 0 \right). $$
**Функция начального магнитного поля:**
$$ B_0(r) = 0. $$


#### Параметры симуляции

**Значения:**
* $T = 256$ - число итераций на плазменный период;
- $k = 0.5$ - число ячеек на дебаевскую длину;
- $n_x = 64$ - разрешение сетки по оси $x$;

- $numT = \dfrac{n_x}{2 \sqrt{2} \pi \cdot k}$ - число периодов;

- $density = \dfrac{t}{8 \pi (e^- \cdot l )^2}$ - начальная плотность частиц;  

- $w_p = \sqrt{\dfrac{4\pi \cdot (e^-)^2 * density}{m_e}}$ - плазменная частота;

**Число итераций:**
$$ N = numT \cdot T. $$
**Шаг по времени:**
$$ \Delta t = \dfrac{2\pi}{w_p \cdot T}. $$

#### Используемые модули

Для проведения эксперимента использовался следующий набор модулей (указаны в порядке добавления):

1. `FieldGenerator`;
2. `ParticleGenerator`;
3. `FieldSolver`;
4. `ParticleSolver`;
5. `ParticleLoopEdgeCondition`;
6. `CurrentDepositor`;
7. `DataSampler`.

#### Параметры сбора данных

Для этого следующего эксперимента использовалась следующая конфигурация модуля 
`DataSampler`:

```c++
sampleInterval = 16;

writeParticleGridParameters = true;

sampleTotalEnergy = true;

traceExampleParticle = true;

sampleParticleDensity = true;
sampleParticleEnergy = true;

sampleFieldData = true;
sampleFieldE = true;
sampleFieldB = true;
sampleFieldJ = true;

sampleFieldEnergy = true;

fileForEveryIteration = true;
```


### Результаты эксперимента

После окончания симуляции, выходные файлы собранных данных были обработаны реализованной системой автоматической визуализации. Далее приведены полученные графики.

