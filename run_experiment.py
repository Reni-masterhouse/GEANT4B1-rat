from tqdm import tqdm
import os

experiment_dir = 'exp'
os.makedirs(experiment_dir, exist_ok=True)

config_template = \
'''# Macro file for example B1
# 
# To be run preferably in batch, without graphics:
# exampleB1 runtest.mac
#
/run/numberOfThreads 4
/run/initialize
#
/control/verbose 0
/run/verbose 0

# # run1
# proton {0} to the direction (0.,0.,1.)
# 1000000 events
#
/gun/particle proton
/gun/energy {0} 
#
/run/beamOn 1000000
'''

energies = [
    '100 MeV',
    '90 MeV',
    '80 MeV',
    '70 MeV',
    '60 MeV',
    '50 MeV',
    '40 MeV',
    '30 MeV',
    '20 MeV',
    '10 MeV',
    '5 MeV',
]

with tqdm(energies) as iters:
    for energy in iters:
        runmac_path = os.path.join(experiment_dir, f'myruntest_{energy}.mac')
        runmac_path = runmac_path.replace(' ', '_')

        log_path = os.path.join(experiment_dir, f'mylog_{energy}.txt')
        log_path = log_path.replace(' ', '_')
        command = f'./exampleB1 {runmac_path} > {log_path}'

        iters.set_postfix({
            'energy': energy,
            'command': command,
        })


        with open(runmac_path, 'w') as f:
            f.write(config_template.format(energy))
        
        code = os.system(command)
        if code != 0:
            print('some error in executing', command)
            break