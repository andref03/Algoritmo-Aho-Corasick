# Aho-Corasick

Este repositório contém uma implementação do algoritmo Aho-Corasick para correspondência de padrões em uma sequência de texto. A implementação está no arquivo `ahoCorasick.cpp`, e a principal modificação ocorre na linha 143, onde os padrões de busca são definidos.

## Como usar

1. Abra o arquivo `ahoCorasick.cpp` em um editor de código.

2. Vá para a linha 143 e altere o vetor `patterns` para incluir os padrões que você deseja buscar:

    ```cpp
    vector<string> patterns = {"seu", "novo", "conjunto", "de", "padroes"};
    ```

3. Salve as alterações no arquivo.

## Sobre

O algoritmo Aho-Corasick permite a busca de padrões diversos de maneira simultânea devido à árvore de prefixos (ou árvore trie). O objetivo principal é semelhante ao algoritmo KMP no quesito de buscar padrões em arquivos de texto, porém Aho-Corasick é mais eficiente para buscas de muitos padrões aos mesmo tempo. Essa otimização, em relação ao KMP, pode ser vista no arquivo PDF anexado, que contém os slides apresentados. Acesse o arquivo `slides.pdf`, vá na sessão "Análise de Desempenho" e na sessão "Conclusões", para visualizar as comparações feitas.
