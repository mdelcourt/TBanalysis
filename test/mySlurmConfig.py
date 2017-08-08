from CP3SlurmUtils.Configuration import Configuration

config = Configuration()

#--------------------------------------------------------------------------------
# 1. SLURM sbatch command options
#--------------------------------------------------------------------------------

config.sbatch_partition = 'cp3'
config.sbatch_qos = 'cp3'
config.sbatch_workdir = './slurm/'
config.sbatch_time = '0-01:00:00'
config.sbatch_mem = '1024'

config.sbatch_output = ''
config.sbatch_error = ''

# Example: config.sbatch_additionalOptions = ['--mail-type=END', '--mail-user=<your-email-address>']
config.sbatch_additionalOptions = []

#--------------------------------------------------------------------------------
# 2. User batch script parameters that are same for all jobs
#--------------------------------------------------------------------------------

config.environmentType = ''

# The input files, apart from input data in the storage, needed by the jobs.
# Do NOT include archive files (tarballs) in this list.
# A tarball (input sandbox) will be created containing all the files in this list.
# The tarball will be copied to, and unpacked in, the scratch directory of each job.
# Do NOT use absolute paths. Keep in mind also that relative paths are not only to locate the files locally,
# but also when the tarball is unpacked the relative paths are respected. That is, if you add in the input sandbox a line "mydir/*.txt",
# then all txt files present in ./mydir will be included in the input sandbox and when unpacked in the worker nodes
# you will have a directory mydir with all these txt files inside.
# Defaults to an empty list (i.e. no input sandbox).
config.inputSandboxContent = ["pipeline.py","../bin/*.out","../Dict_rdict.pcm","../include/*.h"]

config.inputSandboxDir = config.sbatch_workdir
config.inputSandboxFilename = ''
config.batchScriptsDir = config.sbatch_workdir
config.batchScriptsFilename = ''

config.stageout = True
config.stageoutFiles = ["*.root"]
config.stageoutDir = config.sbatch_workdir

config.writeLogsOnWN = True
config.separateStdoutStderrLogs = False
config.stdoutFilename = "slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out"
config.stderrFilename = ""

config.stageoutLogs = True
config.stageoutLogsDir = config.stageoutDir + '/logs'

config.useJobArray = True
config.numJobs = None

#--------------------------------------------------------------------------------
# 3 Job-specific input parameters and payload
#--------------------------------------------------------------------------------

# Job input parameters will be passed to the job via bash environment variables.
# There will be one variable per input parameter (e.g. if each of your jobs needs
# N input parameters, there will be N bash environment variables defined in each job).
# You will have to provide the variable names in the configuration parameter 'inputParamsNames'.
# The same variable names will be used for all jobs, while their values will be set per job.
# Then it is the user responsability to correctly use these variables in his/ber payload.
# The payload is a set of bash lines (which can execute programs in any other language of course).
# So one way to retrieve the input variables is to do it in the bash lines of the payload.
# The way to access a variable in bash is with the following syntax: ${variable-name}.

# This configuration parameter should be a list containing the names of the job input parameters.
# It is important that the names in this list and the values in the 'inputParams' list
# follow the same order (the slurm_submit tool will assume that this is the case).
# Defaults to an empty list (i.e. no input parameters).
config.inputParamsNames = ["fileId"]

# This configuration parameter should be a list where each element is itself a list containing
# the values of the job input parameters.
# It is important that the values in this list and the names in the 'inputParamsNames' list
# follow the same order (the slurm_submit tool will assume that this is the case).
# Defaults to an empty list (i.e. no input parameters).
config.inputParams = [[i] for i in range(9)]

# The payload is code that you want the job to run. It consists of a set of bash lines.
# In principle, it can be as simple as "./my_code" (remember to put the file 'my_code' in the input sandbox)
# and inside 'my_code' you do whatever you want.
# For CMS jobs, the payload can be for example "cmsRun my_code".
# (Remember: for CMS jobs, the `cmsenv` command does not need to be included in the payload;
# if 'cmsswDir' is specified, `cmsenv` will be executed before running the payload inside that directory).
# No validation of the content of this parameter is done.
# If you want to propagate an error inside the payload to the job exit code, make sure the payload
# exits with the right exit code. Following the suggestion in http://www.tldp.org/LDP/abs/html/exitcodes.html
# user-defined exit codes should be in the range 79-113, but we reserved the range 100-113
# for batch script errors. Payload exit codes in the range 100-113 will be propagated
# as job exit code 103 (the general exit code used by the batch script for signaling error in user payload).
# This is a schematic example of how to use exit codes in a payload:
# config.payload = \
# """
# do something ...
# if `do something ...` failed
#    exit 79
# do something else ...
# if `do something else ...` failed
#     exit 80
# exit 0
# """
# This parameter is mandatory and has no default value.
config.payload = \
    """
    module load root/6.06.02-sl6_gcc49
    python pipeline.py
    """
