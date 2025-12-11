import matplotlib.pyplot as pyplot
from pathlib import Path
from dataclasses import dataclass, field
from enum import Enum
from PIL import Image



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
class GlobalData:
    field_energy : list[list[int, float]] = field(default_factory=list)

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
    "ParticleDensity" : list(),
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
    pyplot.legend(loc='lower left')
    
    # Ensure the save directory exists
    path = Path(save_location)
    directory = path.parent
    directory.mkdir(parents=True, exist_ok=True)
    
    # Save the plot
    pyplot.savefig(save_location)
    pyplot.close()
    
    
def build_per_file_plots(file_data : FileData):
    
    if file_data.has_particle_density:  
        x_values = [entry[0] for entry in file_data.particle_density]
        v_values = [entry[2] for entry in file_data.particle_density]
        
        image_path = f"./Output/ParticleDensity/ParticleDensity_{file_data.iteration}.png"
        build_plot(x_values, v_values, image_path, f"ParticleDensity_{file_data.iteration}")
        generated_plot_image_lists["ParticleDensity"].append([file_data.iteration, image_path])
        
    if file_data.has_field_E:   
        x_values = [entry[0] for entry in file_data.field_E]
        v_x_values = [entry[2].x for entry in file_data.field_E]
        v_y_values = [entry[2].y for entry in file_data.field_E]
        v_z_values = [entry[2].z for entry in file_data.field_E]
        
        image_path = f"./Output/FieldE/Field_E_x/FieldE_x_{file_data.iteration}.png"
        build_plot(x_values, v_x_values, image_path, f"Field_E_x_{file_data.iteration}")
        generated_plot_image_lists["Field_E_x"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldE/Field_E_z/FieldE_y_{file_data.iteration}.png"
        build_plot(x_values, v_y_values, image_path, f"Field_E_y_{file_data.iteration}")
        generated_plot_image_lists["Field_E_y"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldE/Field_E_z/FieldE_z_{file_data.iteration}.png"
        build_plot(x_values, v_z_values, image_path, f"Field_E_z_{file_data.iteration}")
        generated_plot_image_lists["Field_E_z"].append([file_data.iteration, image_path])
        
        
    if file_data.has_field_B:   
        x_values = [entry[0] for entry in file_data.field_B]
        v_x_values = [entry[2].x for entry in file_data.field_B]
        v_y_values = [entry[2].y for entry in file_data.field_B]
        v_z_values = [entry[2].z for entry in file_data.field_B]
        
        image_path = f"./Output/FieldB/Field_B_x/FieldB_x_{file_data.iteration}.png"
        build_plot(x_values, v_x_values, image_path, f"Field_B_x_{file_data.iteration}")
        generated_plot_image_lists["Field_B_x"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldB/Field_B_y/FieldB_y_{file_data.iteration}.png"
        build_plot(x_values, v_y_values, image_path, f"Field_B_y_{file_data.iteration}")
        generated_plot_image_lists["Field_B_y"].append([file_data.iteration, image_path])
        
        image_path = f"./Output/FieldB/Field_B_z/FieldB_z_{file_data.iteration}.png"
        build_plot(x_values, v_z_values, image_path, f"Field_B_z_{file_data.iteration}")
        generated_plot_image_lists["Field_B_z"].append([file_data.iteration, image_path])
        
        
    if file_data.has_field_B:   
        x_values = [entry[0] for entry in file_data.field_J]
        v_x_values = [entry[2].x for entry in file_data.field_J]
        v_y_values = [entry[2].y for entry in file_data.field_J]
        v_z_values = [entry[2].z for entry in file_data.field_J]
        
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
    
    # Building field energy plot
    x_values = [entry[0] for entry in global_data.field_energy]
    y_values = [entry[1] for entry in global_data.field_energy]
    
    build_plot(x_values, y_values, "./Output/FieldEnergy.png", "FieldEnergy")


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
        for file in files:
            file_data = parse_file(file, global_data)       
            build_per_file_plots(file_data)
            
        build_file_plot_gifs("./Output")
        build_global_plots(global_data)
        
        output_path = Path("./Output")
        print("Results saved into: ", output_path.absolute())


# Script entry point
if __name__ == "__main__":
    main()

    