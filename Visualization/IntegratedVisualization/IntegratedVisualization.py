import matplotlib.pyplot as pyplot
from pathlib import Path
from dataclasses import dataclass, field
from enum import Enum
from PIL import Image
import os



class ParsingState(Enum):
    GENERAL = 0
    PARTICLE_DENSITY = 1
    FIELD_DATA = 2

@dataclass
class Vector3:
    x : float = 0
    y : float = 0
    z : float = 0
    
@dataclass
class ParticleData:
    x: float
    y: float
    vx : float
    vy : float
    
@dataclass
class ParticleGridData:
    enabled : bool = False
    resolutionX: int = 0
    resolutionY: int = 0
    deltaX: float = 0
    deltaY: float = 0
    originX: float = 0
    originY: float = 0

@dataclass
class GlobalData:
    field_energy : list[list[int, float]] = field(default_factory=list)
    particle_energy : list[list[int, float]] = field(default_factory=list)
    total_energy : list[list[int, float]] = field(default_factory=list)
    
    particle_samples : dict[dict[ParticleData]] = field(default_factory=dict)
    particle_grid_Data : ParticleGridData = field(default_factory=ParticleGridData)

@dataclass
class FileData:
    
    # Tags
    inverse_x : bool = False
    inverse_y : bool = False
    # only_x : bool = False
    # only_y : bool = False
    
    iteration : int = 0
    
    # Data presence flags
    has_particle_density : bool = False
    has_field_E : bool = False
    has_field_B : bool = False
    has_field_J : bool = False
    
    # Data
    
    particle_density : list[ list[float, float, float] ] = field(default_factory=list)
    
    field_E : list[ list[float, float, Vector3] ] = field(default_factory=list)
    field_B : list[ list[float, float, Vector3] ] = field(default_factory=list)
    field_J : list[ list[float, float, Vector3] ] = field(default_factory=list)


# Global variables

generated_plot_image_lists : dict[str : list[list[int, str]]] = {
    "ParticleDensity_x" : list(),
    "ParticleDensity_y" : list(),
    "ParticleDensity_heat" : list(),
    "Field_E_x" : list(),
    "Field_E_y" : list(),
    "Field_E_z" : list(),
    "Field_B_x" : list(),
    "Field_B_y" : list(),
    "Field_B_z" : list(),
    "Field_J_x" : list(),
    "Field_J_y" : list(),
    "Field_J_z" : list()
}


# Parsing
def parse_file(file_path : str, out_global_data : GlobalData) -> FileData:
    
    # Output data
    file_data = FileData()
    
    # Parsing state
    parsing_state : ParsingState = ParsingState.GENERAL
    
    with open(file_path) as file:
        for line in file.readlines():
    
            line = line.strip()
    
            # Skipping empty lines
            if line.count(' ') == len(line):
                continue
            
            # Parsing tags
            if line.startswith("#"):
                if line == "#InverseX\n": file_data.inverse_x = True
                elif line == "#InverseY\n": file_data.inverse_y = True
                    
            # Parsing actual data
            else:
                
                if line.startswith("Iteration: "):
                    file_data.iteration = int(line.replace("Iteration: ", ""))
                    
                elif line.startswith("Field Energy: "):
                    out_global_data.field_energy.append([file_data.iteration, float(line.replace("Field Energy: ", ""))])
                    
                elif line.startswith("Particle Energy: "):
                    out_global_data.particle_energy.append([file_data.iteration, float(line.replace("Particle Energy: ", ""))])
                    
                elif line.startswith("Total Energy: "):
                    out_global_data.total_energy.append([file_data.iteration, float(line.replace("Total Energy: ", ""))])
                    
                # Parsing particle grid parameters
    
                elif line.startswith("Particle Grid Parameters:"):
                    out_global_data.particle_grid_Data.enabled = True
                
                elif line.startswith("Resolution X:"):
                    line = line.replace("Resolution X: ", "")
                    out_global_data.particle_grid_Data.resolutionX = int(line)
                
                elif line.startswith("Resolution Y:"):
                    line = line.replace("Resolution Y: ", "")
                    out_global_data.particle_grid_Data.resolutionY = int(line)
                    
                elif line.startswith("Delta X:"):
                    line = line.replace("Delta X: ", "")
                    out_global_data.particle_grid_Data.deltaX = float(line)
                    
                elif line.startswith("Delta Y:"):
                    line = line.replace("Delta Y: ", "")
                    out_global_data.particle_grid_Data.deltaY = float(line)
                    
                elif line.startswith("Origin: "):
                    line = line.replace("Origin: ", "")
                    parts = line.split(",")
                    out_global_data.particle_grid_Data.originX = float(parts[0])
                    out_global_data.particle_grid_Data.originY = float(parts[1])
                    
                    
                # Parsing particle data
                elif " | " in line:
                    
                    parts = line.split(" | ")
                    
                    particleID = int(parts[0])
                    line = parts[1]
                    
                    if line.startswith("Location: "):
                        
                        # Ensure dict contains data for this particle ID
                        if not particleID in out_global_data.particle_samples:
                            out_global_data.particle_samples[particleID] = {}
                        
                        line = line.replace("Location: ", "")
                        parts = line.split(",")
                        x = float(parts[0])
                        y = float(parts[1])
                        
                        if file_data.iteration not in out_global_data.particle_samples[particleID]:
                            out_global_data.particle_samples[particleID][file_data.iteration] = ParticleData(0, 0, 0, 0)
                        
                        out_global_data.particle_samples[particleID][file_data.iteration].x = x
                        out_global_data.particle_samples[particleID][file_data.iteration].y = y
                            
                    elif line.startswith("Velocity: "):
                        
                        # Ensure dict contains data for this particle ID
                        if not particleID in out_global_data.particle_samples:
                            out_global_data.particle_samples[particleID] = {}
                        
                        line = line.replace("Velocity: ", "")
                        parts = line.split(",")
                        vx = float(parts[0])
                        vy = float(parts[1])
                        
                        if file_data.iteration not in out_global_data.particle_samples[particleID]:
                            out_global_data.particle_samples[particleID][file_data.iteration] = ParticleData(0, 0, 0, 0)
                        
                        out_global_data.particle_samples[particleID][file_data.iteration].vx = vx
                        out_global_data.particle_samples[particleID][file_data.iteration].vy = vy
                    
                
                elif line == "Particle Density:":
                    parsing_state = ParsingState.PARTICLE_DENSITY
                    file_data.has_particle_density = True
                    
                elif line == "Field Data:":
                    parsing_state = ParsingState.FIELD_DATA
                
                # Parsing particle density
                elif parsing_state == ParsingState.PARTICLE_DENSITY:
                    
                    coord, value = line.split(" : ")
                    x, y = [float(i) for i in coord.split(", ")]
                    
                    file_data.particle_density.append([x, y, float(value)])
                    
                elif parsing_state == ParsingState.FIELD_DATA:
                    
                    if line.startswith("E: ") or line.startswith("B: ") or line.startswith("J: "):
                        
                        val_type = ""
                        
                        if line.startswith("E: "): 
                            val_type = "E"
                            file_data.has_field_E = True
                            
                        if line.startswith("B: "): 
                            val_type = "B"
                            file_data.has_field_B = True
                            
                        if line.startswith("J: "): 
                            val_type = "J"
                            file_data.has_field_J = True
                        
                        line = line.replace("E: ", "").replace("B: ", "").replace("J: ", "")
                        
                        coord, value = line.split(" : ")
                        lx, ly = [float(i) for i in coord.split(", ")]
                        vx, vy, vz = [float(i) for i in value.split(", ")]
                        vector_value = Vector3(vx, vy, vz)
                        data = [lx, ly, vector_value]
                        
                        if val_type == "E": file_data.field_E.append(data)
                        if val_type == "B": file_data.field_B.append(data)
                        if val_type == "J": file_data.field_J.append(data)
                
    return file_data     
                

# Building a plot
def build_plot(x_values : list[float], y_values : list[float], save_location : str, plot_label = "Plot"):
    
    pyplot.figure()
        
    # Creating the plot
    pyplot.plot(x_values, y_values, label=plot_label)
    pyplot.title(plot_label)
    pyplot.legend(loc='lower left')
    
    # Ensure the save directory exists
    path = Path(save_location)
    directory = path.parent
    directory.mkdir(parents=True, exist_ok=True)
    
    # Save the plot
    pyplot.savefig(save_location)
    pyplot.close()
    
    
def build_heat_map(x : list, y : list, v : list, save_location : str, plot_label = "Plot"):
    
    pyplot.figure()
    
    # Creating the heat map
    pyplot.tricontourf(x, y, v, levels=200, cmap='viridis')
    pyplot.colorbar(label=plot_label)
    pyplot.title(plot_label)
    pyplot.xlabel("X")
    pyplot.ylabel("Y")
    
    # Ensure the save directory exists
    path = Path(save_location)
    directory = path.parent
    directory.mkdir(parents=True, exist_ok=True)
    
    # Save the plot
    pyplot.savefig(save_location)
    pyplot.close()
    
    
def build_trajectory_plot(global_data : GlobalData, save_location : str):
    
    # Plotting particle grid
    if global_data.particle_grid_Data.enabled:
        
        gridXValues = [
            global_data.particle_grid_Data.originX + i * global_data.particle_grid_Data.deltaX
            for i in range(global_data.particle_grid_Data.resolutionX)
        ]
        
        gridYValues = [
            global_data.particle_grid_Data.originY + j * global_data.particle_grid_Data.deltaY
            for j in range(global_data.particle_grid_Data.resolutionY)
        ]
        
        for x in gridXValues:
            pyplot.axvline(x=x, color='lightgray', linestyle='--', linewidth=0.5)
        
        for y in gridYValues:
            pyplot.axhline(y=y, color='lightgray', linestyle='--', linewidth=0.5)

    # Plotting the trajectories

    for particleID in global_data.particle_samples:
        
        iterations = list(global_data.particle_samples[particleID].keys())
        iterations.sort()
        
        x_values = [i for i in iterations]
        samples = global_data.particle_samples[particleID]
        
        xValues = [samples[i].x for i in iterations]
        yValues = [samples[i].y for i in iterations]
        
        pyplot.plot(xValues, yValues, label=f"Particle {particleID}", linewidth=1)
        pyplot.scatter(xValues, yValues, s=2)
        
        if (len(xValues) > 1):
            for i in range(0, len(xValues) - 1, len(xValues) // 10):
                pyplot.annotate('', xy=(xValues[i + 1], yValues[i + 1]), xytext=(xValues[i], yValues[i]),
                    arrowprops=dict(color='grey', lw=0.5))
                
    pyplot.title("Particle Trajectories")
    pyplot.xlabel("X Position")
    pyplot.ylabel("Y Position")
    pyplot.legend()
    
    # Ensure the save directory exists
    path = Path(save_location)
    directory = path.parent
    directory.mkdir(parents=True, exist_ok=True)
    
    # Save the plot
    pyplot.savefig(save_location)
    pyplot.close()

    
    
def build_per_file_plots(file_data : FileData):
    
    if file_data.has_particle_density:
        x_values = list(set(entry[0] for entry in file_data.particle_density))
        x_values.sort()
        v_values = [sum( [entry[2] for entry in file_data.particle_density if entry[0] == x] ) for x in x_values]

        image_path = f"./Output/ParticleDensity_x/ParticleDensity_x_{file_data.iteration}.png"
        build_plot(x_values, v_values, image_path, f"ParticleDensity_x_{file_data.iteration}")
        generated_plot_image_lists["ParticleDensity_x"].append([file_data.iteration, image_path])
        
        # image_path = f"./Output/ParticleDensity_y/ParticleDensity_y_{file_data.iteration}.png"
        # build_plot(y_values, v_values, image_path, f"ParticleDensity_y_{file_data.iteration}")
        # generated_plot_image_lists["ParticleDensity_y"].append([file_data.iteration, image_path])
        
        # # For heat map
        # try:
        #     data = np.array(file_data.particle_density)
        #     heat_x = data[:, 0]
        #     heat_y = data[:, 1]
        #     heat_v = data[:, 2]
        
        #     image_path = f"./Output/ParticleDensity_heat/ParticleDensity_heat_{file_data.iteration}.png"
        #     build_heat_map(heat_x, heat_y, heat_v, image_path, f"ParticleDensity_{file_data.iteration}")
        #     generated_plot_image_lists["ParticleDensity_heat"].append([file_data.iteration, image_path])
            
        # except Exception as e:
        #     print(f"Failed to build heat map: {str(e)}")
        
        
    if file_data.has_field_E:       
        x_values = list(set(entry[0] for entry in file_data.field_E))
        x_values.sort()
        v_x_values = [sum( [entry[2].x for entry in file_data.field_E if entry[0] == x] ) for x in x_values]
        v_y_values = [sum( [entry[2].y for entry in file_data.field_E if entry[0] == x] ) for x in x_values]
        v_z_values = [sum( [entry[2].z for entry in file_data.field_E if entry[0] == x] ) for x in x_values]
        
        image_path = f"./Output/FieldE/Field_E_x/FieldE_x_{file_data.iteration}.png"
        build_plot(x_values, v_x_values, image_path, f"Field_E_x_{file_data.iteration}")
        generated_plot_image_lists["Field_E_x"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldE/Field_E_y/FieldE_y_{file_data.iteration}.png"
        build_plot(x_values, v_y_values, image_path, f"Field_E_y_{file_data.iteration}")
        generated_plot_image_lists["Field_E_y"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldE/Field_E_z/FieldE_z_{file_data.iteration}.png"
        build_plot(x_values, v_z_values, image_path, f"Field_E_z_{file_data.iteration}")
        generated_plot_image_lists["Field_E_z"].append([file_data.iteration, image_path])
        
        
    if file_data.has_field_B:   
        x_values = list(set(entry[0] for entry in file_data.field_B))
        x_values.sort()
        v_x_values = [sum( [entry[2].x for entry in file_data.field_B if entry[0] == x] ) for x in x_values]
        v_y_values = [sum( [entry[2].y for entry in file_data.field_B if entry[0] == x] ) for x in x_values]
        v_z_values = [sum( [entry[2].z for entry in file_data.field_B if entry[0] == x] ) for x in x_values]
        
        image_path = f"./Output/FieldB/Field_B_x/FieldB_x_{file_data.iteration}.png"
        build_plot(x_values, v_x_values, image_path, f"Field_B_x_{file_data.iteration}")
        generated_plot_image_lists["Field_B_x"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldB/Field_B_y/FieldB_y_{file_data.iteration}.png"
        build_plot(x_values, v_y_values, image_path, f"Field_B_y_{file_data.iteration}")
        generated_plot_image_lists["Field_B_y"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldB/Field_B_z/FieldB_z_{file_data.iteration}.png"
        build_plot(x_values, v_z_values, image_path, f"Field_B_z_{file_data.iteration}")
        generated_plot_image_lists["Field_B_z"].append([file_data.iteration, image_path])
        
        
    if file_data.has_field_J:   
        x_values = list(set(entry[0] for entry in file_data.field_J))
        x_values.sort()
        v_x_values = [sum( [entry[2].x for entry in file_data.field_J if entry[0] == x] ) for x in x_values]
        v_y_values = [sum( [entry[2].y for entry in file_data.field_J if entry[0] == x] ) for x in x_values]
        v_z_values = [sum( [entry[2].z for entry in file_data.field_J if entry[0] == x] ) for x in x_values]
        
        image_path = f"./Output/FieldJ/Field_J_x/FieldJ_x_{file_data.iteration}.png"
        build_plot(x_values, v_x_values, image_path, f"Field_J_x_{file_data.iteration}")
        generated_plot_image_lists["Field_J_x"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldJ/Field_J_y/FieldJ_y_{file_data.iteration}.png"
        build_plot(x_values, v_y_values, image_path, f"Field_J_y_{file_data.iteration}")
        generated_plot_image_lists["Field_J_y"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldJ/Field_J_z/FieldJ_z_{file_data.iteration}.png"  
        build_plot(x_values, v_z_values, image_path, f"Field_J_z_{file_data.iteration}")
        generated_plot_image_lists["Field_J_z"].append([file_data.iteration, image_path])



def build_global_plots(global_data : GlobalData):
    
    global_data.field_energy.sort(key=lambda x: x[0])
    global_data.particle_energy.sort(key=lambda x: x[0])
    global_data.total_energy.sort(key=lambda x: x[0])
    
    # Building field energy plot
    x_values = [entry[0] for entry in global_data.field_energy]
    y_values = [entry[1] for entry in global_data.field_energy]
    
    build_plot(x_values, y_values, "./Output/FieldEnergy.png", "FieldEnergy")
    
    # Building particle energy plot
    x_values = [entry[0] for entry in global_data.particle_energy]
    y_values = [entry[1] for entry in global_data.particle_energy]
    
    build_plot(x_values, y_values, "./Output/ParticleEnergy.png", "ParticleEnergy")
    
    # Building total energy plot
    x_values = [entry[0] for entry in global_data.total_energy]
    y_values = [entry[1] for entry in global_data.total_energy]
    
    build_plot(x_values, y_values, "./Output/TotalEnergy.png", "TotalEnergy")
    
    if (len(global_data.particle_samples) > 0):
        build_trajectory_plot(global_data, "./Output/ParticleTrace.png")
        
        for particle_id in global_data.particle_samples:
            
            iterations = list(global_data.particle_samples[particle_id].keys())
            iterations.sort()
            
            x_values = [i for i in iterations]
            #y_values = [(((data.vx * data.vx) + (data.vy * data.vy)) ** 0.5) for data in global_data.particle_samples[particle_id]]
            samples = global_data.particle_samples[particle_id]
            y_values = [(((samples[i].vx * samples[i].vx) + (samples[i].vy * samples[i].vy)) ** 0.5) for i in iterations]
            build_plot(x_values, y_values, f"./Output/ParticleSpeed_{particle_id}.png", f"ParticleSpeed_{particle_id}")
        

def generate_gif(images : list, output_path : str):

    # Ensure the save directory exists
    path = Path(output_path)
    directory = path.parent
    directory.mkdir(parents=True, exist_ok=True)
    
    images[0].save(output_path, save_all=True, append_images=images[1:], optimize=False, duration=400, loop=0)
    

def build_file_plot_gifs(output_path : str):
    for name, image_paths in generated_plot_image_lists.items():
        if len(image_paths) > 0:
            image_paths.sort(key=lambda x: x[0])
            images = [Image.open(path[1]) for path in image_paths]
            generate_gif(images, output_path=f"{output_path}/{name}.gif");


def main():
    # Locating files
    path = Path("./Files")
            
    # Checking if specified path exits
    if path.exists():

        global_data = GlobalData()

        # Gathering all files from the directory
        files = [f for f in path.rglob("*") if f.is_file()]

        # Looping over files and passing them to the CodeScanner
        for i, file in enumerate(files):
            print(f"{(float(i + 1) / len(files) * 100):.1f}% : Processing file: '{file}'")
            file_data = parse_file(file, global_data)       
            build_per_file_plots(file_data)
            
        build_file_plot_gifs("./Output")
        build_global_plots(global_data)
        
        output_path = Path("./Output")
        print("Results saved into: ", output_path.absolute())
        
        # Clearing data files, so further experiments with less iterations are processed correctly
        for file in files:
            os.remove(file)


# Script entry point
if __name__ == "__main__":
    main()

    