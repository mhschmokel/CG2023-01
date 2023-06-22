# Trabalho GB

O trabalho consistem em um visualizador de objetos 3D a partir de um arquivo de inicialização .ini

## Mapeamento do mouse e teclas
### Mouse
- Botão direito do mouse: desabilita câmera e habilita cursor para interagir com a janela de load de .obj
- Botão esquerdo do mouse: habilita câmera e desabilita cursor

### Teclado
- W, A, S, D: movimento de câmera
- Arrows Up, Down, Left, Right: movimento de translação do objeto nos eixos X e Y
- [, ] (ou '`', dependendo do mapeamento do teclado): movimento de translação do objeto no eixo Z
- R: reset do objeto para a posição e tamanhos originais
- Page Up (PgUp) e Page Down (PgDn): aumenta e diminui a escala do objeto
- SPACE: exibe todos os objetos carregador no vetor de objetos ou retorna a visualização para o objeto atual
- N: troca o objeto para o próximo objeto carregado no vetor de objetos
- X: inicia ou interrompe a rotação do objeto no eixo X
- Y: inicia ou interrompe a rotação do objeto no eixo Y
- Z: inicia ou interrompe a rotação do objeto no eixo Z

## Configuração do arquivo .ini

O arquivo funciona de maneira semelhante à um arquivo de materiais (.mtl), constituído em IDs e seus valores. O programa possui suporte para os seguintes IDs:

- `object`: caminho do arquivo .obj que será carregado na cena (1 valor do tipo string)
- `rotation`: rotação do objecto no eixo Y. (1 valor do tipo float)
- `position`: posição do objeto na cena. (3 valores do tipo float)
- `scale`: escala do objeto. (3 valores do tipo float)
- `lightPos`, `lightColor`: atributos da fonte de luz. (3 valores do tipo float para cada um)
- `lightSpecular`: atributo da fonte de luz. (1 valor do tipo int)
- `cameraPosition`, `cameraFront`, `cameraUp`: atributos da câmera. (3 valores do tipo float para cada um)
- `cameraSpeed`: velocidade da câmera. (1 valor do tipo float)

## Dependências:

- ![ImGUI](https://github.com/ocornut/imgui)
- ![ImGUI File Browser](https://github.com/AirGuanZ/imgui-filebrowser)

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
