package smc.semanticAnalyzer;

import java.util.*;

public class SemanticStateMachine {
  public final List<AnalysisError> errors = new ArrayList<>();
  public final List<AnalysisError> warnings = new ArrayList<>();
  public final SortedMap<String, SemanticState> states = new TreeMap<>();
  public final Set<String> events = new HashSet<>();
  public final Set<String> actions = new HashSet<>();
  public SemanticState initialState;
  public String actionClass;
  public String fsmName;

  public String toString() {
    return String.format(
            """
                    Actions: %s
                    FSM: %s
                    Initial: %s\
                    %s""",
      actionClass, fsmName, initialState.name, statesToString());

  }

  public void addError(AnalysisError analysisError) {
    errors.add(analysisError);
  }

  public String statesToString() {
    String statesString = "{";
    for (SemanticState s : states.values()) {
      statesString += s.toString();
    }
    return statesString + "}\n";
  }

  public static class SemanticState implements Comparable<SemanticState> {
    public final String name;
    public final List<String> entryActions = new ArrayList<>();
    public final List<String> exitActions = new ArrayList<>();
    public boolean abstractState = false;
    public final SortedSet<SemanticState> superStates = new TreeSet<>();
    public final List<SemanticTransition> transitions = new ArrayList<>();

    public SemanticState(String name) {
      this.name = name;
    }

    public boolean equals(Object obj) {
      if (obj instanceof SemanticState other) {
          return
          Objects.equals(other.name, name) &&
            Objects.equals(other.entryActions, entryActions) &&
            Objects.equals(other.exitActions, exitActions) &&
            Objects.equals(other.superStates, superStates) &&
            Objects.equals(other.transitions, transitions) &&
            other.abstractState == abstractState;
      } else
        return false;
    }

    public String toString() {
      return
        String.format("\n  %s {\n%s  }\n",
          makeStateNameWithAdornments(),
          makeTransitionStrings());
    }

    private String makeTransitionStrings() {
      String transitionStrings = "";
      for (SemanticTransition st : transitions)
        transitionStrings += makeTransitionString(st);

      return transitionStrings;
    }

    private String makeTransitionString(SemanticTransition st) {
      return String.format("    %s %s {%s}\n", st.event, makeNextStateName(st), makeActions(st));
    }

    private String makeActions(SemanticTransition st) {
      String actions = "";
      boolean firstAction = true;
      for (String action : st.actions) {
        actions += (firstAction ? "" : " ") + action;
        firstAction = false;
      }
      return actions;
    }

    private String makeNextStateName(SemanticTransition st) {
      return st.nextState == null ? "null" : st.nextState.name;
    }

    private String makeStateNameWithAdornments() {
      String stateName = "";
      stateName += abstractState ? ("(" + name + ")") : name;
      for (SemanticState superState : superStates)
        stateName += " :" + superState.name;
      for (String entryAction : entryActions)
        stateName += " <" + entryAction;
      for (String exitAction : exitActions)
        stateName += " >" + exitAction;
      return stateName;
    }

    public int compareTo(SemanticState s) {
      return name.compareTo(s.name);
    }
  }

  public static class AnalysisError {
    public enum ID {
      NO_FSM,
      NO_INITIAL,
      INVALID_HEADER,
      EXTRA_HEADER_IGNORED,
      UNDEFINED_STATE,
      UNDEFINED_SUPER_STATE,
      UNUSED_STATE,
      DUPLICATE_TRANSITION,
      ABSTRACT_STATE_USED_AS_NEXT_STATE,
      INCONSISTENT_ABSTRACTION,
      STATE_ACTIONS_MULTIPLY_DEFINED,
      CONFLICTING_SUPERSTATES,
    }

    private final ID id;
    private Object extra;

    public AnalysisError(ID id) {
      this.id = id;
    }

    public AnalysisError(ID id, Object extra) {
      this(id);
      this.extra = extra;
    }

    public String toString() {
      return String.format("Semantic Error: %s(%s)", id.name(), extra);
    }

    public int hashCode() {
      return Objects.hash(id, extra);
    }

    public boolean equals(Object obj) {
      if (obj instanceof AnalysisError other) {
          return id == other.id && Objects.equals(extra, other.extra);
      }
      return false;
    }
  }

  public static class SemanticTransition {
    public String event;
    public SemanticState nextState;
    public final List<String> actions = new ArrayList<>();
  }
}
