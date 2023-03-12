
EXE=./Build/KEDIT.exe

run:$(EXE)
	$(EXE)

clean:
	$(shell powershell "Remove-Item .\Build -Recurse -Force")

$(EXE):
	cmake -G Ninja -B Build
	cd Build && ninja


