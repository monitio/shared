# Contribution Guidelines
> Thank you for your interest in contributing to a project that is a part of Monitio!
> We are dedicated to the mission of improving existing software and innovating on new ideas.
>
> To maintain our core values, we need transparent high-quality standards across all projects
> and welcome your commitment to following them.
>
> By contributing to Monitio, you also agree to follow our [Community Standards (Code of Conduct)](CODE_OF_CONDUCT.md).

## 💖 Contribution is Optional
Contributing to Monitio is entirely **optional**. We understand that not everyone has the time or experience (though this is a great place to learn) to submit code or fixes and that's perfectly fine. You can contribute in many valuable ways that don't involve writing code, such as:
- **Giving feedback**: You can provide your [opinion](https://en.wikipedia.org/wiki/Opinion) on anything you want (e.g., features, designs and documentation). **We strongly encourage respectful disagreement and constructive criticism.**
- **Sorting**: Reviewing and classifying existing issues and pull requests.
- **Support**: Helping other users in forums or community channels.
- **Outreach**: Sharing Monitio projects and encouraging others to participate.

Your choice of participation, in any form, is what drives the community forward.

## Getting started
To guide your entry into open source contribution, here are the foundational resources that explain the ethos and process:
- [Why contribute to open source?](https://opensource.guide/how-to-contribute/#why-contribute-to-open-source)
- [What it means to contribute](https://opensource.guide/how-to-contribute/#what-it-means-to-contribute)
- [Orienting yourself to a new project](https://opensource.guide/how-to-contribute/#orienting-yourself-to-a-new-project)

1. Identify a need
  - Before starting work on the need, please check the project's issue tracker for the following:
    - **Found a bug?** Check if an issue already exists. If not, open a new one using the appropriate template.
    - **Proposing a feature?** Open an issue to discuss the new feature or improvement with the maintainers first. This prevents unnecessary work and ensures alignment with the project's goals.
2. Fork and branch
  - Fork the repository and create a new, descriptive branch for your changes (e.g, `fix/bug-description` or `feat/new-component-name`).

## Code submission standards
All contributions, regardless of size, must strictly meet the following standards.
### **A. Typed Commit Message Standard**
Monitio uses a custom **Type-Based Commit Message** format (similar to Conventional Commits, Python functions and `TODO` comments) for standardization. This convention is critical because the **Type** determines the version increment when a release is cut, which then follows [the official SemVer-Typing specification](VERSIONING.md). This format is for **commit messages only**.

**Format**: `type(scope): subject`

| Type       | Description                                                                             | Version Bump |
|:-----------|:----------------------------------------------------------------------------------------|:-------------|
| `feat`     | New feature for the user.                                                               | **MINOR**    |
| `fix`      | A fix for a bug.                                                                        | **PATCH**    |
| `docs`     | Documentation only changes.                                                             | None         |
| `style`    | Formatting, missing semicolons, etc. (no code change).                                  | None         |
| `refactor` | Refactoring code without changing public API.                                           | None         |
| `test`     | Adding missing tests or correcting existing tests.                                      | None         |
| `chore`    | Maintenance tasks, configuration changes, etc.                                          | None         |
| `BCHANGE`  | Must be included in the commit body if the change is major and non-backward compatible. | **MAJOR**    |

*(`BCHANGE` is short for a **breaking change**.)*
**Example commit messaage:**
```
BCHANGE(main): First proper full commit.
feat(api): Make new time API for developers to use
```

#### **Release versioning policy**
While the commit messages follow the structure above, the official release versioning format ([SemVer-Typing](https://github.com/monitio/semver-typing)) that is applied to tags and project releases is defined in the project-specific [`VERSIONING.md` file](VERSIONING.md). All releases **must** meet this official standard.

### **B. Code quality and formatting**
We prioritize **simple code** and **direct execution**. All code must be automatically formatted using the standard Monitio configuration.
1. **Indentation**: All code must use **two (2) spaces** for indentation in every format. **Tabs or using four (4) spaces for indentation are both forbidden.**
  - This is enforced automatically using our standard [`templates/.prettierrc`](templates/.prettierrc) configuration and by manually changing your editor config to use two (2) spaces for indentation. Please ensure you are running Prettier before submitting your PR.
2. **Linting**: Resolve all warnings and errors reported by the project's linter.
3. **Simplicity**: Write code that is easy to read, debug and maintain. Avoid overly complex logic when a simpler solution exists. *(This obviously doesn't apply to already overly complicated math-physics equations as it is incredibly hard to debug them.)*

### **C. Testing requirements**
We understand that writing tests takes effort, but tests are critical for maintaining the stability of Monitio projects. While we try to reach for high test coverage, please focus on these core areas:
- **New features**: Please include tests that demonstrate your new functionality works as intended. This helps future maintainers understand its behavior. Commits that are added straight to the main repo without a pull reqeust by a core maintainer, **do not require** tests but would be greatly appreciated.
- **Bug fixes**: If you are fixing a bug, please include a small test that replicates the bug and then confirms it is fixed by your change. This prevents the bug from reappearing later.

