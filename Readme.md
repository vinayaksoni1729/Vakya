# 🌿 Vākya DSL Specification (v1.1)

Vākya is a compact, expressive, and declarative domain-specific language (DSL) designed to write structured prompts for large language models (LLMs). It aims to reduce prompt ambiguity and improve response consistency while remaining intuitive and minimal.

---

## ✨ Overview

- **Declarative** and **step-based** with support for chaining actions
- Focused on tasks like search, summarization, transformation, comparison, etc.
- Produces clear and verbose natural language (NL) prompts for LLMs to execute reliably
- User-friendly syntax with concise, consistent keywords

---

## ⌘ Keywords and Blocks

### ✅ Step Block

```
@steps {
  @do <action>              # Action to perform (e.g., research, summarize, compare)
  @src <source list>        # Where to gather info from
  @fmt <format>(<columns>)  # Output format and fields
  -> <transformations>      # Sorting, grouping, limits
  @cdn { <conditions> }     # Filtering conditions
  ;;
  ...                       # More steps
}

```

### Step Syntax (Optional @steps)

You can skip the `@steps` block and write steps sequentially:

```
@do research #gadget_search
@src reddit, google
@fmt @table(name, price, link) -> @asc(price) [ #tone=formal ] ;;

@do summarize
@on #gadget_search
@fmt @paragraph(summary, verdict) [ #tone=analytical ] ;;

```

### System Keywords

| Keyword | Purpose |
| --- | --- |
| `@steps` | Block containing ordered steps (optional) |
| `@do` | Action verb (e.g., summarize) |
| `@src` | List of sources to query |
| `@fmt` | Output format and fields |
| `->` | Output transformations |
| `@cdn` | Logical conditions (filters) |
| `@on` | Reference a previous step via step name |

---

## ✏ Syntax Rules

### Actions (`@do`)

Use action verbs such as:

- `research`, `compare`, `summarize`, `analyze`, `convert`, `rewrite`, `highlight`
- You may attach a step name using `#`: `@do summarize #step2`

### Sources (`@src`)

- No prefix for fixed sources: `reddit`, `youtube`, `wikipedia`
- Use `@` prefix for predefined source groups: `@all`, `@retail`, `@news`

### Output Format (`@fmt`)

```
@fmt @table(name, price:currency, rating?)

```

- Formats can be:
    - `@table`, `@paragraph`, `@bullet_points`, `@list`, etc.
- Type hints allowed via `:` (e.g., `price:currency`)
- Use `#` for custom user-defined formats: `#mind_map(...)`
- Column suffixes can indicate importance:
    - `!` → Must include
    - *(none)* → Should include
    - `?` → Optional (include if possible)

### Output Modifiers (`>`)

```
-> @asc(price), @dsc(rating), @grp(category), @limit(5)

```

- `@asc(col)` → Sort ascending
- `@dsc(col)` → Sort descending
- `@grp(col)` → Group by
- `@limit(n)` → Limit rows/items

### Metadata Block `[ ... ]`

```
[ #tone=professional, #vibe=calm ]

```

- For style, tone, theme, etc.
- `#` prefix indicates LLM-interpreted soft hints

### Conditions (`@cdn`)

```
@cdn {
  ! price >= 1000 AND brand = Realme
    rating > 4
  ? release_date > 2022 OR warranty > 1
}

```

- Prefix `!` = Must-have (high priority)
- No prefix = Should-have (medium priority)
- Prefix `?` = Nice-to-have (low priority)
- Use `AND`, `OR`, and `(...)` for logic
- Prefix appears **once per condition expression**, not per clause

---

## 📘 Example

```
@do compare #top5_phones
@src amazon, flipkart, @retail
@fmt @table(name!, price:currency, rating?)
-> @grp(brand), @asc(rating), @limit(5)
[ #tone=neutral, #theme=minimal ]
@cdn {
  ! price >= 1000 AND brand = Realme
    rating > 4
  ? release_date > 2022 OR warranty > 1
}
;;

@do summarize
@on #top5_phones
@fmt @paragraph(summary, highlights)
[ #tone=insightful ]
;;

```

---

## 🎓Natural Language Query

```reason
Step 1: Compare Phones

Please research and compare smartphones available on Amazon, Flipkart, and other major retail platforms.

Filter results to include only phones that cost at least ₹1000 and belong to the brand Realme.

Prefer phones that have a rating greater than 4, and if possible, also prefer models that were released after 2022 or offer more than 1 year of warranty.

Organize the results in a table format with the following columns:

1. Name (this column is required and must be present),

2. Price (in currency format),

3. Rating (optional; include it if available).

Group the entries by brand,

Sort them in ascending order of rating,

Show only the top 5 entries based on these criteria.

Maintain a neutral tone and a minimal presentation style in the output.

---

Step 2: Summarize the Comparison

Now, based on the data from the comparison above (from step #top5_phones), generate a paragraph-style summary.

This paragraph should:

Present a concise summary of the key findings,

Highlight any noteworthy insights or trends observed in the selected top 5 phones.

Use an insightful tone in your writing.

```

---

# 📊 Word Count Comparison

Section	DSL Words	NL Words	Explanation

Step 1	53	153	Includes action, source, format, condition, tone, etc.
Step 2	21	59	Includes summarization instruction with tone
Total	74	212	~2.9x verbosity increase in NL

---

# ⏱️ Time to Write Comparison (Estimate)

Type	Time (avg user)	Notes

DSL	~45–60 seconds	Depends on user familiarity; structured, auto-completable
NL Prompt	~2.5–4 minutes	Requires careful writing to ensure clarity and completeness

# 🧠 Syntax Notes

- `@grp`, `@asc`, `@dsc`, `@limit` are all output logic operators → appear in `>`
- Metadata is always enclosed in `[...]`
- User-defined formats/fields use `#` prefix
- All keywords lowercase with `@` prefix
- `@do <action> #stepname` defines a reusable step name
- `@on #stepname` allows referencing previous step outputs
- `@cdn` conditions respect priority prefixes: `!`, none, `?`
- Column names in `@fmt` can use `!`, `?` suffixes to denote requiredness

---

## 🔧 Roadmap Features (v2+)

- Nested steps & subqueries
- Inline variable passing between steps
- User-defined macros and format aliases
- Custom condition aliases (e.g., `@cdn #cheap_phone`)
- Parser error feedback and intelligent suggestions

---

## 🔖 Naming

**Vākya** (Sanskrit: वाक्य) means *sentence, expression, or utterance*, signifying its purpose as a compact language that expresses full-fledged thoughts and intentions for intelligent machines.
