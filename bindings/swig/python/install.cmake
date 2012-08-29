EXECUTE_PROCESS(COMMAND
	python setup.py install
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bindings/swig/python"
)
