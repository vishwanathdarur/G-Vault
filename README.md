# G-Vault
Please note: [Work in Progress] The current project is being actively worked on.

gvault is a high-performance JSON query engine built to experiment with compiler front-ends (query lexing, parsing, semantic analysis), IR optimization, bytecode generation, and virtual machine execution using lazy data streaming.

## Current Progress
What is already working (and what is next):

- [x] Baseline JSON Lexer and Recursive Descent Parser
- [ ] Custom Query Language (GQL) Lexer and Parser
- [ ] Semantic analysis and type checking for queries
- [ ] Intermediate Representation (IR) generation
- [ ] Optimization passes (Constant folding, Predicate reordering)
- [ ] Custom Bytecode emission
- [ ] Stack-based Virtual Machine (VM) execution
- [ ] Projection pruning (lazy JSON streaming based on query needs)
- [ ] IR and Bytecode inspection from the CLI

## Visuals
A visual of `data.json` demonstrating the query lexing, parsing, and emitted IR:

*[query-ir-demo gif placeholder]*

A visual demonstrating the full pipeline: projection pruning the JSON, optimizing the query, and executing the VM bytecode:

*[execution-demo gif placeholder]*

## Pipeline
Current stages:

1. Query Lexing
2. Query Parsing
3. Semantic Analysis
4. Raw IR Generation
5. Optimized IR Generation
6. Bytecode Emission
7. Lazy JSON Ingestion (Projection Pruning)
8. VM Execution

## Build
```bash
/path/to/cmake -S . -B build
/path/to/cmake --build build
```

## Run
Run a query against a JSON file:

```bash
./build/gvault --run "SELECT name WHERE age > 20" examples/data.json
```

Useful flags:

- `--tokens` prints query lexer output
- `--ast` prints the parsed query AST
- `--semantics` prints the AST after type checking
- `--dump-ir` prints the intermediate representation before optimization
- `--dump-opt-ir` prints the IR after constant folding and reordering
- `--dump-bytecode` prints the generated VM bytecode instructions
- `--extra-verbose` prints every intermediate stage and execution time

## Layout
- `src` - compiler and VM implementation
- `include` - headers
- `examples` - sample JSON data and queries
- `docs` - query syntax notes, IR notes, and bytecode specs

## Notes
- See `docs/syntax.md` for the GQL language syntax
- See `docs/bytecode.md` for the stack VM instruction set
- See `docs/optimizations.md` for details on projection pruning and constant folding
