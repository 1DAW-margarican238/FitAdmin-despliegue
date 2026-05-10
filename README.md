# FitAdmin

FitAdmin es una plataforma SaaS para la gestión integral de gimnasios. Cada gimnasio que se registra tiene su propio espacio completamente independiente: sus socios, su personal, sus datos y hasta su identidad visual (logo y colores corporativos). Lo que hace un gimnasio no afecta al resto.

Está pensada para cubrir el día a día de cualquier gimnasio: gestión de socios y membresías, control de equipamiento e incidencias, administración del personal con distintos niveles de acceso, pagos online con Stripe, y generación automática de rutinas de entrenamiento personalizadas mediante inteligencia artificial.


# Características principales

- Multi-tenant — cada gimnasio opera de forma completamente aislada
- Personalización visual — cada gimnasio elige su logo y colores corporativos, que se aplican automáticamente a toda la interfaz
- 4 roles de usuario — SuperAdmin, Admin, Empleado y Socio, con permisos granulares por rol
- Rutinas con IA — generación automática de rutinas de entrenamiento personalizadas usando Groq
- Pagos con Stripe — gestión de membresías y suscripciones con webhooks para actualización automática
- Correos transaccionales — emails automáticos de bienvenida, aprobación y comunicaciones masivas via Resend
- Módulos opcionales — pistas de pádel con sistema de reservas y clases grupales con control de aforo (Spinning, Crossfit, Piscina...)
- Panel de estadísticas — gráficas y métricas en tiempo real para admins


# Stack tecnológico
Frontend → React + Vite + SASS
Backend → Node.js + Express
Base de datos → Supabase (PostgreSQL)
Autenticación → JWT
Pagos → Stripe
IA → Groq API
Emails → Resend
Despliegue → Docker + Railway


# Arquitectura general
Frontend → React + Vite + SASS (Puerto 5173)
↓ HTTP / REST API

Backend → Node.js + Express (Puerto 3001)
→ Autenticación JWT
→ Gestión de socios
→ Máquinas y módulos
→ Rutinas con IA
→ Stripe Webhooks
→ Sistema de emails

Base de datos → Supabase (PostgreSQL)
APIs externas → Stripe + Groq API + Resend
Despliegue → Docker + Railway

El frontend se comunica con el backend mediante una API REST. El backend gestiona toda la lógica de negocio y se conecta a Supabase como base de datos y a las APIs externas según la funcionalidad.


# Modelo de datos

El esquema completo está alojado en Supabase (PostgreSQL).

# Tablas
gym -> Gimnasios registrados en la plataforma, con sus colores, logo y claves de Stripe
users -> Todos los usuarios del sistema (SuperAdmin, Admin, Empleado y Socio)
role -> Los 4 roles disponibles en el sistema
user_role -> Relación muchos a muchos entre usuarios y roles
membership_type -> Tipos de membresía creados por cada gimnasio (mensual, trimestral, anual...)
member_membership -> Membresía activa de cada socio, con fechas, estado y datos de pago de Stripe
machine -> Inventario de máquinas del gimnasio con su estado y guía de uso generada por IA
incident -> Incidencias reportadas sobre máquinas
training_routine -> Rutinas de entrenamiento generadas por IA, almacenadas en JSON
exercise -> Catálogo de ejercicios con músculos, instrucciones e imágenes
workout_session -> Sesiones de entrenamiento registradas por cada socio
workout_set -> Series completadas dentro de una sesión (peso, repeticiones)
access_log -> Registro de accesos al gimnasio por socio
accesos -> Tabla auxiliar de control de acceso por UID
registration_request -> Solicitudes de registro de nuevos gimnasios y socios pendientes de aprobación
module_requests -> Peticiones de módulos extra enviadas por los gimnasios al SuperAdmin
gym_modules -> Módulos activos para cada gimnasio (Pádel, Spinning, Crossfit...)
module_courts -> Pistas del módulo de pádel
module_court_bookings -> Reservas de pistas de pádel por socio
module_classes -> Clases grupales con aforo, horario e instructor
module_class_enrollments -> Inscripciones de socios a clases grupales
module_posts -> Publicaciones o avisos dentro de un módulo
saas_plan_config -> Configuración de los planes de FitAdmin vinculados a Stripe


# Relaciones principales

gym              1 ── N  users
gym              1 ── N  machine
gym              1 ── N  membership_type
gym              1 ── N  gym_modules
gym              1 ── N  module_requests
gym              1 ── N  registration_request
users            N ── M  role  (a través de user_role)
users            1 ── N  member_membership
users            1 ── N  training_routine
users            1 ── N  workout_session
users            1 ── N  access_log
machine          1 ── N  incident
membership_type  1 ── N  member_membership
training_routine 1 ── N  workout_session
workout_session  1 ── N  workout_set
gym_modules      1 ── N  module_courts
gym_modules      1 ── N  module_classes
gym_modules      1 ── N  module_posts
module_courts    1 ── N  module_court_bookings
module_classes   1 ── N  module_class_enrollments


# Repositorios

El proyecto está dividido en tres repositorios:

- fitadmin-frontend -> (https://github.com/2DAW-jorodia0603/frontend-FitAdmin) — interfaz React
- fitadmin-backend-> (https://github.com/2DAW-jorodia0603/backend-FitAdmin) — API REST Node.js
- fitadmin-despliegue-> (https://github.com/1DAW-margarican238/FitAdmin-despliegue) — este repo, con el Docker Compose para levantar todo en local


# Instrucciones de despliegue local

# Requisitos previos

- [Docker](https://www.docker.com/) instalado y corriendo
- [Git](https://git-scm.com/) instalado

# 1. Preparar la estructura de carpetas

Clona los tres repositorios dentro de una misma carpeta raíz:

mkdir fitadmin && cd fitadmin
git clone https://github.com/1DAW-margarican238/FitAdmin-despliegue.git .
git clone https://github.com/2DAW-jorodia0603/frontend-FitAdmin.git frontend-FitAdmin
git clone https://github.com/2DAW-jorodia0603/backend-FitAdmin.git backend-FitAdmin


# La estructura tiene que quedar así:
fitadmin/
├── docker-compose.yml          ← de este repo
├── frontend-FitAdmin/          ← repo del frontend
└── backend-FitAdmin/           ← repo del backend


# 2. Configurar las variables de entorno

Frontend - crea un archivo `.env` dentro de `frontend-FitAdmin/` con la siguiente línea:

VITE_API_URL=http://localhost:3001/api


Backend — crea un archivo `.env` dentro de `backend-FitAdmin/` con las credenciales que están en el documento de entrega en la tarea:
PORT=3001
SUPABASE_URL=
SUPABASE_ANON_KEY=
SUPABASE_SERVICE_ROLE_KEY=
GROQ_API_KEY=
RAPIDAPI_KEY=
RESEND_API_KEY=
RESEND_FROM=FitAdmin <onboarding@resend.dev>
NODE_ENV=development




# 3. Levantar el proyecto

Desde la carpeta raíz (`fitadmin/`), ejecuta:

docker-compose up --build


Esto construye las imágenes y arranca los dos servicios. La primera vez tarda un poco más.

Una vez arrancado:

- Frontend → [http://localhost:5173](http://localhost:5173)
- Backend → [http://localhost:3001](http://localhost:3001)


---

## Equipo

Proyecto de fin de grado — 2º DAW 2025/2026
- Pablo Martín Toledano
- Javier Orozco Díaz
- Margarita Cantero Durán