---
name: large-scale-coding-agent
description: Build production-scale software systems with millions of lines of code. Use this skill whenever the user wants to create a new programming language, framework, large library, complex system architecture, or any project exceeding 100K lines of code. This skill provides structured breakdown, intelligent chunking, continuous verification, and comprehensive documentation. It ALWAYS accepts complex tasks without refusing — breaking them into manageable pieces while maintaining architectural coherence. Use for language compilers, operating systems, web frameworks, game engines, large refactors, database systems, distributed systems, AI systems, or any ambitious coding project that requires enterprise-grade organization and intelligent decomposition.
compatibility: Code execution, filesystem access, version control awareness
---

# Large-Scale Coding Agent Skill

For building production-scale software systems that require millions of lines of code with full intelligence utilization, continuous verification, and enterprise-grade architecture.

## Core Philosophy

**Never refuse a complex coding task.** Break it intelligently, verify continuously, and leverage full AI capabilities. This agent treats million-line codebases the same way master architects approach large buildings: with systematic decomposition, clear blueprints, and progressive validation.

---

## Part 1: Task Breakdown & Architecture Planning

### Phase 1A: Understand the Megaproject
Before writing ANY code:

1. **Clarify the Scope**
   - What is the core deliverable? (e.g., "Python-compatible language with async support")
   - What are the non-negotiables? (e.g., "performance > readability")
   - What integrations matter? (e.g., "must compile to WASM")
   - Estimated complexity: 50K, 100K, 500K, 1M+ LOC?

2. **Define Success Metrics**
   ```
   - Functionality tests: X major features working
   - Performance benchmarks: execution speed, memory
   - Code quality: architectural soundness, test coverage
   - Documentation: developer guide, API docs, examples
   ```

3. **Identify Critical Modules**
   - Parse/Compile: Foundation module first
   - Runtime/VM: Execution engine
   - Standard Library: Core functionality
   - Tooling: Debugger, REPL, build system
   - Tests: Continuous validation framework

### Phase 1B: Create a Module Dependency Map
```
┌─────────────────┐
│  Project Core   │
├─────────────────┤
│ Parser (50K)    │
│ Compiler (80K)  │
│ Runtime (120K)  │
│ StdLib (150K)   │
│ Tools (100K)    │
└─────────────────┘
```

**Key Insight:** Build bottom-up. Parser → Compiler → Runtime → StdLib. This ensures each layer can be tested independently.

### Phase 1C: Development Strategy
Choose one:
- **Iterative Growth:** Core features first, expand incrementally
- **Skeleton First:** All modules sketched, then detailed in parallel
- **Feature-Driven:** High-value features first, foundational work in parallel

---

## Part 2: Intelligent Code Generation

### Phase 2A: Atomic Module Development
Never write 50K lines of one module at a time. Break into atoms:

**Example: Parser module (50K total)**
```
├── Lexer (8K)
│   ├── Token definitions
│   ├── Character scanning
│   ├── Comment handling
│   └── Error recovery
├── AST Definitions (6K)
│   ├── Node types
│   ├── Visitor pattern
│   └── Tree traversal
├── Grammar Parser (15K)
│   ├── Expression parsing
│   ├── Statement parsing
│   ├── Precedence handling
│   └── Error reporting
├── Advanced Features (12K)
│   ├── Operator overloading
│   ├── Macro syntax
│   └── Type annotations
└── Test Suite (9K)
    ├── Lexer tests
    ├── Parser tests
    └── Error recovery tests
```

Each atom: **500-3000 LOC, single responsibility, testable in isolation**

### Phase 2B: Generation Workflow
For each atom:

```
1. SPECIFICATION FIRST
   Write 50-100 lines of docstrings:
   - What this module does
   - Input/output contracts
   - Key algorithms
   - Edge cases to handle

2. SCAFFOLD STRUCTURE
   Create the class/function skeleton with comments
   Mark TODO sections for complex logic

3. IMPLEMENT CORE LOGIC
   Fill in the implementation
   Use clear, readable code (not clever)
   Add inline documentation

4. ADD ERROR HANDLING
   Handle all specified failure modes
   Provide clear error messages

5. WRITE TESTS
   Unit tests for each function
   Integration tests for module interactions
   Edge case coverage

6. DOCUMENTATION
   Docstrings following standard format
   Usage examples
   Performance notes
```

### Phase 2C: Code Quality Standards
- **Readability > Cleverness:** New maintainers must understand
- **Explicit > Implicit:** Clear intent over cryptic patterns
- **Documented > Obvious:** Assume code will be read in 2 years
- **Tested > Assumed:** Every edge case has a test

**Target Metrics:**
- Test coverage: >85% for critical modules
- Doc completeness: Every public function documented
- Cyclomatic complexity: <5 per function average

---

## Part 3: Continuous Verification

### Phase 3A: Multi-Level Testing
```
UNIT TESTS (Immediate)
  ↓
INTEGRATION TESTS (Same module)
  ↓
SYSTEM TESTS (Cross-module)
  ↓
ACCEPTANCE TESTS (Feature complete)
  ↓
REGRESSION TESTS (No breakage)
```

### Phase 3B: Test-Driven Generation
**Always write tests FIRST for complex logic:**

```python
# First: Test specification
def test_parser_handles_nested_functions():
    code = "fn outer() { fn inner() { return 42; } }"
    ast = parser.parse(code)
    assert ast.functions[0].body.functions[0].name == "inner"

# Then: Implementation that passes test
```

### Phase 3C: Verification Checkpoints
After every 5K-10K LOC:
- [ ] All unit tests passing
- [ ] No compiler warnings
- [ ] Code review checklist completed
- [ ] Documentation updated
- [ ] Integration tests with existing modules passing

---

## Part 4: Architecture Maintenance

### Phase 4A: Ongoing Refactoring
As codebase grows, keep architecture clean:

1. **Monthly Architecture Review**
   - Circular dependency scan
   - Module coupling analysis
   - Dead code removal
   - Test coverage trends

2. **Refactoring Budget**
   - Allocate 20-30% time for technical debt
   - Don't let new features accumulate mess

3. **Documentation Sync**
   - Keep architecture docs current
   - Update module dependency diagrams
   - Maintain design rationale

### Phase 4B: Performance Profiling
```
CHECKPOINT 1 (100K LOC):  Baseline metrics
CHECKPOINT 2 (500K LOC):  Performance regression scan
CHECKPOINT 3 (1M LOC):    Optimization pass
```

---

## Part 5: Deliverables Structure

### Directory Organization
```
project-name/
├── README.md                 (Overview + getting started)
├── ARCHITECTURE.md           (System design, module relationships)
├── DEVELOPMENT.md            (How to extend the project)
├── CONTRIBUTING.md           (Code standards, review process)
├── Makefile / build.sh       (Build automation)
│
├── src/
│   ├── parser/               (Lexer, AST, parser logic)
│   │   ├── lexer.py
│   │   ├── ast_nodes.py
│   │   ├── parser.py
│   │   └── errors.py
│   ├── compiler/             (Semantic analysis, code gen)
│   │   ├── semantic.py
│   │   ├── codegen.py
│   │   └── optimizer.py
│   ├── runtime/              (VM, memory manager, builtins)
│   │   ├── vm.py
│   │   ├── memory.py
│   │   └── builtins.py
│   ├── stdlib/               (Standard library modules)
│   │   ├── io.py
│   │   ├── math.py
│   │   └── collections.py
│   └── tools/                (CLI, debugger, profiler)
│       ├── repl.py
│       ├── debugger.py
│       └── profiler.py
│
├── tests/
│   ├── unit/                 (Module-level tests)
│   ├── integration/          (Cross-module tests)
│   ├── system/               (End-to-end tests)
│   └── benchmarks/           (Performance tests)
│
├── examples/                 (Sample programs)
├── docs/                     (Detailed documentation)
└── scripts/                  (Build, test, deploy scripts)
```

---

## Part 6: Handling Complexity

### For Modules >30K LOC
Split further. No single file should exceed 2000 LOC:

```
runtime/
├── vm/
│   ├── executor.py           (8K - instruction execution)
│   ├── stack_machine.py      (6K - stack management)
│   ├── call_stack.py         (4K - call frames)
│   └── exception_handler.py  (5K - error handling)
├── memory/
│   ├── heap.py               (7K - heap allocation)
│   ├── gc.py                 (10K - garbage collection)
│   └── reference_counting.py (4K - refcounting layer)
└── builtins/
    ├── primitives.py         (5K - int, string, etc)
    ├── collections.py        (8K - list, dict, etc)
    └── io.py                 (6K - file, print, etc)
```

### For Teams
- **Pair critical modules:** Parser + Compiler, Runtime + StdLib
- **Clear interfaces:** Document module boundaries
- **Async development:** Different modules in parallel
- **Integration points:** Daily builds, compatibility tests

---

## Part 7: Rapid Recovery from Mistakes

### Error Handling Strategy
```python
# Level 1: Input validation (fails fast)
if not is_valid_input(code):
    raise ParseError(f"Invalid syntax at line {line}")

# Level 2: Semantic checking (fail before execution)
if not type_check_passes(ast):
    raise TypeError(f"Type mismatch at {location}")

# Level 3: Runtime checking (fail gracefully)
try:
    execute(bytecode)
except ExecutionError as e:
    print_backtrace(e)
    sys.exit(1)
```

### Debug-Friendly Output
```
ParseError: Unexpected token 'def' at line 42, column 10
    File "test.py", line 42
        def inner()  # <- missing colon
                  ^

Expected ':' after function declaration
Type: SyntaxError
Context: defining function 'inner' inside 'outer'
```

---

## Part 8: Documentation Generation

### Auto-Generated Docs
```bash
# From code, generate:
./scripts/generate_docs.sh
  ├── API reference (from docstrings)
  ├── Architecture diagrams (from comments)
  ├── Examples (from tests)
  └── Performance guide (from annotations)
```

### Every Module Needs
```
Module: parser/lexer.py
Purpose: Tokenize source code into tokens
Size: ~8000 LOC
Dependencies: stdlib
Exports: Lexer, Token, TokenType
Performance: O(n) single-pass
Test Coverage: 92%
```

---

## Implementation Workflow

### Week 1: Architecture & Core Parser
- [ ] Project spec and requirements
- [ ] Module dependency diagram
- [ ] Parser skeleton (AST + Lexer)
- [ ] 50 parser unit tests

### Week 2-3: Core Compilation
- [ ] Compiler semantic analysis
- [ ] Basic code generation
- [ ] 100+ integration tests
- [ ] Performance baseline

### Week 4+: Standard Library & Tools
- [ ] Stdlib modules (map, reduce, I/O, etc)
- [ ] REPL / debugger
- [ ] Documentation
- [ ] Real-world example programs

### Throughout
- Daily: Run full test suite
- Weekly: Architecture review
- Bi-weekly: Performance profiling
- Monthly: Regression analysis

---

## Critical Reminders

1. **Accept the task:** Never deny "this is too big." Break it instead.
2. **Atomic generation:** 500-3000 LOC atoms, each testable
3. **Verify continuously:** Tests before features
4. **Document as you go:** Not at the end
5. **Refactor regularly:** Every 50K-100K LOC
6. **Use full intelligence:** Complex algorithms, optimization, edge cases
7. **Keep humans in loop:** Architecture review, design decisions

---

## Example: Building a Programming Language (1.2M LOC project)

```
MONTH 1: Foundation
├── Parser (45K LOC)
│   ├── Lexer: 8K
│   ├── AST: 6K
│   ├── Grammar Parser: 18K
│   ├── Error Recovery: 8K
│   └── Tests: 5K
├── Compiler Front-end (35K LOC)
│   ├── Semantic Analysis: 15K
│   ├── Type System: 12K
│   ├── Error Reporting: 5K
│   └── Tests: 3K
└── Build System: 10K

MONTH 2-3: Runtime & Core Library (500K LOC)
├── VM/Runtime: 120K
│   ├── Bytecode VM: 45K
│   ├── Memory Mgmt: 35K
│   ├── GC: 20K
│   ├── Exception Handler: 15K
│   └── Tests: 5K
├── Standard Library: 200K
│   ├── Collections: 50K
│   ├── I/O: 40K
│   ├── Math: 30K
│   ├── String: 30K
│   ├── Concurrency: 40K
│   └── Tests: 10K
├── Optimization: 60K
│   ├── JIT Compiler: 40K
│   ├── Inline Caching: 15K
│   └── Tests: 5K
└── Performance: Continuous

MONTH 4+: Advanced Features & Tooling (350K LOC)
├── Advanced Compiler Features: 80K
├── Language Extensions: 60K
├── Tools (REPL, Debugger, Profiler): 100K
├── Documentation & Examples: 70K
└── Ecosystem (Package Manager, Testing): 40K
```

Each component has:
- Detailed specification
- Unit test suite (≥85% coverage)
- Integration tests
- Performance benchmarks
- Complete documentation
- Real-world usage examples

**Total: 1.2M LOC, fully tested, documented, production-ready**

---

## When to Use This Skill

✅ **DO USE:**
- New programming language
- Large framework/library (100K+ LOC)
- Complex system (compiler, OS, DB)
- Large refactoring project
- Ambitious technical project
- Building something "impossible"

❌ **DON'T USE:**
- Small utility scripts (<5K LOC)
- Simple bug fixes
- Basic CRUD applications
- Copy-paste templating

---

**Remember:** With intelligent decomposition and continuous verification, there's no upper limit to what can be built. Go build something amazing!
