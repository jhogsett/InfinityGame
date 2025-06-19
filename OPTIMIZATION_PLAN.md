# InfinityGame Optimization Plan

## Project Status Assessment

### Current Resource Usage
- **Flash Memory**: ~100% utilized (32KB limit on ATmega328P)
- **RAM**: Low enough to cause stack crashes (2KB limit)
- **Source Code**: 91KB across 35 .cpp files
- **Headers**: 33KB across 36 .h files

### Critical Issues Identified
1. **Memory Constraints**: Flash nearly full, RAM causing stack overflows
2. **Code Duplication**: Significant repeated patterns across game modules
3. **Potential Bugs**: Memory management and timing issues

---

## Phase 1: String and Flash Memory Optimization (PRIORITY 1)

### 1.1 PROGMEM String Optimization
**Status**: In Progress  
**Risk**: Low  
**Expected Flash Savings**: 2-5KB  

**Issues Found**:
- Inconsistent use of F() macro for string literals
- Multiple sprintf_P calls that could use PROGMEM strings
- Repeated string constants not consolidated

**Actions**:
- [x] ~~Audit all string literals for PROGMEM usage~~ 
- [x] ~~Create centralized string constants file~~ **COMPLETED**
- [x] ~~Convert repeated strings in play_views.cpp~~ **COMPLETED**
- [ ] Convert sprintf_P calls in options_mode.cpp to use constants
- [ ] Update remaining files to use string constants
- [ ] Test build size after each batch of changes

**NOTES**: Avoided problematic cross-file PROGMEM arrays. Using safer string constants approach.

### 1.2 Remove Debug/Test Code
**Status**: Not Started  
**Risk**: Low  
**Expected Flash Savings**: 1-3KB  

**Issues Found**:
- `#ifdef ENABLE_TEST_FEATURES` sections still compiled
- `#ifdef ENABLE_DEBUG_FEATURES` sections present
- Commented Serial.begin() in main.cpp

**Actions**:
- [ ] Remove or properly ifdef all debug code
- [ ] Verify test mode is completely disabled
- [ ] Clean up commented code sections

### 1.3 Buffer Size Optimization  
**Status**: Not Started  
**Risk**: Medium  
**Expected RAM Savings**: 50-100 bytes  

**Issues Found**:
- `DISPLAY_BUFFER 71` - potentially oversized
- `FSTRING_BUFFER 31` - may be reducible
- `COPY_BUFFER 31` - usage analysis needed

**Actions**:
- [ ] Analyze actual buffer usage patterns
- [ ] Reduce buffer sizes conservatively
- [ ] Test thoroughly on device after each reduction

---

## Phase 2: Code Duplication Elimination (PRIORITY 2)

### 2.1 Game Loop Patterns
**Status**: Not Started  
**Risk**: Medium-High  
**Expected Flash Savings**: 3-8KB  

**Duplicated Patterns Found**:
- Timeout loops: `while ((time = millis()) < timeout)` (6+ instances)
- Button handling sequences
- Display update patterns
- Win/loss processing logic

**Proposed Solution**: Extract common game framework
```cpp
class GameBase {
protected:
    virtual bool playRound() = 0;
    virtual void setupGame() = 0;
    bool handleStandardGameLoop();
    void displayResult(long winAmount);
    bool checkTimeout(unsigned long timeout);
};
```

**Actions**:
- [ ] Create GameBase class header
- [ ] Implement common game loop functionality
- [ ] Convert slot_game.cpp to inherit from GameBase
- [ ] Test slot game thoroughly
- [ ] Convert remaining games one by one

### 2.2 Prompt System Consolidation
**Status**: Not Started  
**Risk**: Medium  
**Expected Flash Savings**: 1-3KB  

**Issues Found**:
- 6 similar timeout loops in prompts.cpp
- Repeated display.loop_scroll_string patterns
- Similar LED management sequences

**Actions**:
- [ ] Extract common prompt loop function
- [ ] Consolidate LED management patterns
- [ ] Refactor prompts.cpp systematically
- [ ] Test all prompt-using functionality

### 2.3 Money/Display Formatting
**Status**: Not Started  
**Risk**: Low  
**Expected Flash Savings**: 500-1000 bytes  

**Issues Found**:
- format_long() calls scattered across 11+ files
- Repeated money display patterns
- Similar sprintf_P formatting calls

**Actions**:
- [ ] Create DisplayFormatter utility class
- [ ] Consolidate money formatting functions
- [ ] Replace scattered format_long calls
- [ ] Test all monetary displays

---

## Phase 3: Bug Fixes and Stability (PRIORITY 3)

### 3.1 Memory Management Issues
**Status**: Not Started  
**Risk**: High  
**Impact**: Stability  

**Bugs Found**:
1. **Buffer Overflows**: sprintf_P calls without bounds checking
2. **Memory Leak**: `new Billboard()` in BillboardsHandler never freed
3. **Stack Overflow**: Deep call chains + interrupt handlers

**Actions**:
- [ ] Add bounds checking to all sprintf_P calls
- [ ] Fix Billboard memory leak in BillboardsHandler
- [ ] Analyze and reduce call stack depth
- [ ] Add stack monitoring for development

### 3.2 Timing and State Issues
**Status**: Not Started  
**Risk**: Medium  
**Impact**: Reliability  

**Bugs Found**:
1. **Button Debouncing**: handle_long_press() in tight loops
2. **Millis Rollover**: No 49-day rollover handling
3. **EEPROM Wear**: Frequent save_data() calls

**Actions**:
- [ ] Review button handling for race conditions
- [ ] Add millis() rollover protection
- [ ] Implement EEPROM write throttling
- [ ] Add state consistency checks

### 3.3 LED State Management
**Status**: Not Started  
**Risk**: Low  
**Impact**: User Experience  

**Issues Found**:
- Some LED deactivation paths might be missed
- Complex LED state interactions

**Actions**:
- [ ] Audit all LED activation/deactivation paths
- [ ] Add LED state validation
- [ ] Simplify LED management patterns

---

## Testing Strategy

### Pre-Change Testing Checklist
Before implementing any changes:
- [ ] Document current functionality
- [ ] Test all game modes (Slots, Word, Time, Code)
- [ ] Test all menu navigation
- [ ] Test options and settings
- [ ] Test LED patterns and sound
- [ ] Document current memory usage if possible

### Post-Change Testing Checklist
After each optimization phase:
- [ ] Build successfully
- [ ] Upload to device successfully
- [ ] Verify all tested functionality still works
- [ ] Check for any new issues or regressions
- [ ] Document memory usage changes
- [ ] Commit changes to version control

### Testing Tools/Methods
- Use VS Code PlatformIO icons for build/upload
- Manual testing on physical device
- Function-by-function verification
- Memory usage monitoring where possible

---

## Implementation Notes

### Conservative Approach
- **One change at a time**: Test each modification individually
- **Rollback ready**: Keep previous working version available
- **Document everything**: Track what works and what doesn't
- **Device testing**: Always verify on actual hardware

### Risk Mitigation
- Start with lowest-risk changes (string optimizations)
- Keep original code commented during transitions
- Test extensively before removing old code
- Maintain development branch for testing

### Success Metrics
- **Flash Usage**: Reduce from ~100% to <85%
- **RAM Usage**: Eliminate stack crashes
- **Functionality**: No regressions in game features
- **Code Quality**: Reduced duplication and improved maintainability

---

## Change Log

| Phase | Date | Change | Flash Δ | RAM Δ | Status | Notes |
|-------|------|---------|---------|-------|---------|-------|
| | | | | | | |

---

## Next Steps

1. **Review this plan** - Confirm approach and priorities
2. **Setup testing baseline** - Document current functionality
3. **Start Phase 1.1** - Begin with string optimization
4. **Test thoroughly** - Verify each change on device
5. **Iterate carefully** - Move through phases systematically

**Estimated Timeline**: 2-4 weeks with careful testing between changes
**Primary Goal**: Stable, optimized codebase with <85% flash usage and no stack crashes
