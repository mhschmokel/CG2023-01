# Trabalho GA

O trabalho consiste em um visualizador de objetos 3D a partir de arquivos .obj.

## Mapeamento do mouse e teclas

### Mouse
- Botão direito do mouse: desabilita câmera e habilita cursor para interagir com a janela de load de .obj
- Botão esquerdo do mouse: habilita câmera e desabilita cursor

### Teclado
- W, A, S, D: movimento de câmera
- Arrows Up, Down, Left, Right: movimento de translação do objeto nos eixos X e Y
- [, ] (ou '`', dependendo do mapeamento do teclado): movimento de translação do objeto no eixo Z
- R: reset do objeto para a posição e tamanhos originais
- K, L (ou + e - do teclado numérico): aumenta e diminui a escala do objeto
- SPACE: exibe todos os objetos carregador no vetor de objetos ou retorna a visualização para o objeto atual
- N: troca o objeto para o próximo objeto carregado no vetor de objetos
- X: inicia ou para a rotação do objeto no eixo X
- Y: inicia ou para a rotação do objeto no eixo Y
- Z: inicia ou para a rotação do objeto no eixo Z

## Dependências:

- ![ImGUI](https://github.com/ocornut/imgui)
- ![ImGUI File Browser](https://github.com/AirGuanZ/imgui-filebrowser)
