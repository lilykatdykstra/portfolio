/* ─── Sticky Nav ─── */
(function () {
  const nav = document.getElementById('nav');
  if (!nav) return;
  const onScroll = () => nav.classList.toggle('scrolled', window.scrollY > 20);
  window.addEventListener('scroll', onScroll, { passive: true });
  onScroll();
})();

/* ─── Mobile Nav Toggle ─── */
(function () {
  const toggle = document.getElementById('navToggle');
  const links  = document.getElementById('navLinks');
  if (!toggle || !links) return;
  toggle.addEventListener('click', () => {
    const open = links.classList.toggle('open');
    toggle.classList.toggle('open', open);
    toggle.setAttribute('aria-expanded', open);
  });
  // Close on outside click
  document.addEventListener('click', (e) => {
    if (!toggle.contains(e.target) && !links.contains(e.target)) {
      links.classList.remove('open');
      toggle.classList.remove('open');
    }
  });
})();

/* ─── Dropdown (click toggle on all screen sizes) ─── */
(function () {
  const dropdowns = document.querySelectorAll('.nav-dropdown');

  dropdowns.forEach(item => {
    const link = item.querySelector(':scope > a');
    link.addEventListener('click', (e) => {
      e.preventDefault();
      const opening = !item.classList.contains('open');
      // Close all first, then open the clicked one if it was closed
      dropdowns.forEach(d => d.classList.remove('open'));
      if (opening) item.classList.add('open');
    });
  });

  // Close open dropdown when clicking anywhere outside it
  document.addEventListener('click', (e) => {
    if (!e.target.closest('.nav-dropdown')) {
      dropdowns.forEach(d => d.classList.remove('open'));
    }
  });
})();

/* ─── IntersectionObserver fade-in ─── */
(function () {
  const targets = document.querySelectorAll('.fade-up, .fade-in');
  const staggerParents = document.querySelectorAll('.stagger');

  const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
      if (entry.isIntersecting) {
        entry.target.classList.add('visible');
        observer.unobserve(entry.target);
      }
    });
  }, { threshold: 0.12, rootMargin: '0px 0px -40px 0px' });

  targets.forEach(el => observer.observe(el));

  // Stagger children
  staggerParents.forEach(parent => {
    const children = parent.children;
    Array.from(children).forEach((child, i) => {
      child.style.transitionDelay = `${i * 80}ms`;
    });
    const staggerObs = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          Array.from(children).forEach(child => child.classList.add('visible'));
          staggerObs.unobserve(entry.target);
        }
      });
    }, { threshold: 0.1, rootMargin: '0px 0px -40px 0px' });
    staggerObs.observe(parent);
  });
})();

/* ─── Projects filter ─── */
(function () {
  const filterBtns = document.querySelectorAll('.filter-btn');
  const cards = document.querySelectorAll('.projects-grid .project-card');
  if (!filterBtns.length) return;

  filterBtns.forEach(btn => {
    btn.addEventListener('click', () => {
      filterBtns.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');

      const filter = btn.dataset.filter;
      cards.forEach(card => {
        const tags = card.dataset.tags || '';
        const show = filter === 'all' || tags.includes(filter);
        if (show) {
          card.removeAttribute('data-hidden');
          card.style.display = '';
        } else {
          card.setAttribute('data-hidden', '');
          card.style.display = 'none';
        }
      });
    });
  });
})();

/* ─── Smooth scroll to footer from "Contact" nav link ─── */
(function () {
  document.querySelectorAll('a[href$="#footer"]').forEach(link => {
    link.addEventListener('click', (e) => {
      const footer = document.getElementById('footer');
      if (footer) {
        e.preventDefault();
        footer.scrollIntoView({ behavior: 'smooth' });
      }
    });
  });
})();

/* ─── Contact form submission ─── */
(function () {
  const form    = document.getElementById('contactForm');
  const success = document.getElementById('formSuccess');
  if (!form) return;
  form.addEventListener('submit', async (e) => {
    e.preventDefault();
    const btn  = form.querySelector('.form-submit');
    const orig = btn.textContent;
    btn.textContent = 'Sending…';
    btn.disabled    = true;
    try {
      const res = await fetch(form.action, {
        method:  'POST',
        body:    new FormData(form),
        headers: { Accept: 'application/json' }
      });
      if (res.ok) {
        form.style.display    = 'none';
        success.style.display = 'flex';
      } else {
        btn.textContent = orig;
        btn.disabled    = false;
      }
    } catch {
      btn.textContent = orig;
      btn.disabled    = false;
    }
  });
})();
